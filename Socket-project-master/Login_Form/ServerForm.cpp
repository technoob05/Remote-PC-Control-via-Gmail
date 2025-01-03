#include "ServerForm.h"
#include "UserInfo.h"
#include "Client.h"
using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Text;
using namespace System::Diagnostics;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::IO;
using namespace UserInfo;
void recordVideo(int duration = 10)
{
    String^ cmd = "ffmpeg -f dshow -video_size 640x480 -framerate 30";
    cmd += " -rtbufsize 100M";
    cmd += " -i video=\"HD Webcam\""; // Thay bằng tên camera của bạn
    cmd += " -t " + duration.ToString(); // Sử dụng ToString() thay vì std::to_string()
    cmd += " -vcodec h264"; // Đổi codec
    cmd += " -preset ultrafast";
    cmd += " -pix_fmt yuv420p"; // Thêm pixel format
    cmd += " -y output.mp4";

    // Sử dụng Process::Start thay vì system()
    Process::Start("cmd.exe", "/c " + cmd);
}
System::Void LoginForm::ServerForm::switchToClientButton_Click(System::Object^ sender, System::EventArgs^ e) {
    this->Hide();
    String^ userId = UserInfo::User::UserId;
    String^ senderEmail = UserInfo::User::SenderEmail;
    String^ password = UserInfo::User::Password;
    String^ receiverEmail = UserInfo::User::ReceiverEmail;
    String^ applicationKey = UserInfo::User::ApplicationKey;
    Client^ clientForm = gcnew Client(receiverEmail, applicationKey, senderEmail, 0);
    clientForm->Show();
}

namespace LoginForm
{

    void ServerForm::UpdateCommunicationLog(String^ message)
    {
        if (this->textBoxServerOutput->InvokeRequired) // Kiểm tra xem có cần Invoke không
        {
            this->textBoxServerOutput->Invoke(gcnew Action<String^>(this, &ServerForm::UpdateCommunicationLog), message);
        }
        else
        {
            this->textBoxServerOutput->AppendText(message + "\r\n");
        }
    }

    // Hàm thực hiện các lệnh nhận được
    void ExecuteCommand(String^ command, ServerForm^ form, NetworkStream^ stream)
    {
        command = command->Trim();
        String^ output;

        // Tìm khoảng trắng đầu tiên
        int spaceIndex = command->IndexOf(' ');

        String^ action;
        String^ fileName = nullptr;

        if (spaceIndex >= 0) {
            // Tách phần action (trước khoảng trắng đầu tiên)
            action = command->Substring(0, spaceIndex);

            // Tách phần fileName (sau khoảng trắng đầu tiên)
            fileName = command->Substring(spaceIndex + 1);
        }
        else {
            // Nếu không có khoảng trắng, toàn bộ là action
            action = command;
        }

        if (command->Equals("LIST_APPS", StringComparison::OrdinalIgnoreCase))
        {
            Process^ processList = gcnew Process();
            processList->StartInfo->FileName = "tasklist";
            processList->StartInfo->UseShellExecute = false;
            processList->StartInfo->RedirectStandardOutput = true;
            processList->Start();

            output = processList->StandardOutput->ReadToEnd();
            processList->WaitForExit();

            form->UpdateCommunicationLog("Running apps:\n" + output);
            array<Byte>^ buffer = Encoding::UTF8->GetBytes(output);
            stream->Write(buffer, 0, buffer->Length);
        }
        else if (command->Equals("LIST_SERVICES", StringComparison::OrdinalIgnoreCase))
        {

            Process^ serviceList = gcnew Process();
            serviceList->StartInfo->FileName = "sc";
            serviceList->StartInfo->Arguments = "query";
            serviceList->StartInfo->UseShellExecute = false;
            serviceList->StartInfo->RedirectStandardOutput = true;
            serviceList->Start();

            output = serviceList->StandardOutput->ReadToEnd();
            serviceList->WaitForExit();

            form->UpdateCommunicationLog("Services:\n" + output);
            array<Byte>^ buffer = Encoding::UTF8->GetBytes(output);
            stream->Write(buffer, 0, buffer->Length);
        }
        else if (command->Equals("SHUTDOWN", StringComparison::OrdinalIgnoreCase))
        {
            try
            {
                String^ response = "Server is shutting down...";
                array<Byte>^ responseData = Encoding::UTF8->GetBytes(response);
                stream->Write(responseData, 0, responseData->Length);

                // Cập nhật log hoặc thực hiện hành động khác nếu cần
                form->UpdateCommunicationLog(response);

                // Thực hiện tắt máy
                Process::Start("shutdown", "/s /t 0");

                // Gửi phản hồi thành công (byte 1)
                array<Byte>^ successResponse = gcnew array<Byte>(1) { 1 };
                stream->Write(successResponse, 0, successResponse->Length);
            }
            catch (Exception^ ex)
            {
                // Gửi phản hồi thất bại (byte 0)
                array<Byte>^ failureResponse = gcnew array<Byte>(1) { 0 };
                stream->Write(failureResponse, 0, failureResponse->Length);

                // Cập nhật log nếu gặp lỗi
                form->UpdateCommunicationLog("Shutdown failed: " + ex->Message);
            }
            stream->Flush(); // Đảm bảo dữ liệu đã được gửi ngay lập tức
        }
        else if (command->Equals("TAKE_SCREENSHOT", StringComparison::OrdinalIgnoreCase))
        { // Tạo một đối tượng Bitmap để lưu ảnh chụp màn hình
            Bitmap^ screenshot = gcnew Bitmap(Screen::PrimaryScreen->Bounds.Width, Screen::PrimaryScreen->Bounds.Height);
            Graphics^ g = Graphics::FromImage(screenshot);

            // Chụp màn hình
            g->CopyFromScreen(0, 0, 0, 0, screenshot->Size);

            // Lưu ảnh vào một MemoryStream dưới định dạng PNG
            MemoryStream^ ms = gcnew MemoryStream();
            screenshot->Save(ms, Imaging::ImageFormat::Png);
            array<Byte>^ imageData = ms->ToArray();

            // Gửi kích thước của mảng byte
            array<Byte>^ sizeBuffer = BitConverter::GetBytes(imageData->Length);
            stream->Write(sizeBuffer, 0, sizeBuffer->Length);
            stream->Flush(); // Đảm bảo kích thước đã được gửi

            // Gửi dữ liệu hình ảnh
            stream->Write(imageData, 0, imageData->Length);
            stream->Flush(); // Đảm bảo dữ liệu hình ảnh đã được gửi

            // Giải phóng tài nguyên
            delete g;
            delete screenshot;
            ms->Close();

            form->UpdateCommunicationLog("Screenshot sent to client.");
        }
        else if (command->Equals("TAKE_PHOTO", StringComparison::OrdinalIgnoreCase))
        {
            // Use Process to capture photo
            Process^ cameraCaptureProcess = gcnew Process();
            cameraCaptureProcess->StartInfo->FileName = "CommandCam";
            cameraCaptureProcess->StartInfo->Arguments = "/filename captured_photo.jpg";
            cameraCaptureProcess->StartInfo->UseShellExecute = false;
            cameraCaptureProcess->Start();
            cameraCaptureProcess->WaitForExit();

            String^ imagePath = "captured_photo.jpg";
            array<Byte>^ imageData = File::ReadAllBytes(imagePath);

            // Gửi kích thước của mảng byte
            array<Byte>^ sizeBuffer = BitConverter::GetBytes(imageData->Length);
            stream->Write(sizeBuffer, 0, sizeBuffer->Length);
            stream->Flush();

            // Gửi dữ liệu hình ảnh
            stream->Write(imageData, 0, imageData->Length);
            stream->Flush();

            form->UpdateCommunicationLog("Photo sent to client.");
        }
        else if (command->Equals("SEND_VIDEO", StringComparison::OrdinalIgnoreCase))
        {
            // List available devices

            String^ record_time = "10";
  
            Process::Start("ffmpeg", "-list_devices true -f dshow -i dummy");

            // Record video using Process
            Process^ ffmpegProcess = gcnew Process();
            ffmpegProcess->StartInfo->FileName = "ffmpeg";
            String^ command_1 = "-f dshow -video_size 640x480 -framerate 30 -rtbufsize 100M -i video=\"HD Webcam\" -t ";
            String^ command_2 = " -vcodec h264 -preset ultrafast -pix_fmt yuv420p -y output.mp4";
            String^ main_command = command_1 + record_time + command_2;
            ffmpegProcess->StartInfo->Arguments = main_command;
            ffmpegProcess->StartInfo->UseShellExecute = false;
            ffmpegProcess->Start();
            ffmpegProcess->WaitForExit();

            String^ videoPath = "output.mp4";
            array<Byte>^ videoData = File::ReadAllBytes(videoPath);
            array<Byte>^ sizeBuffer = BitConverter::GetBytes(videoData->Length);

            // Gửi kích thước trước
            stream->Write(sizeBuffer, 0, sizeBuffer->Length);
            stream->Flush();

            // Gửi toàn bộ video
            stream->Write(videoData, 0, videoData->Length);
            stream->Flush();

            form->UpdateCommunicationLog("Video sent to client.");
        }
        else if (action->Equals("SEND_VIDEO", StringComparison::OrdinalIgnoreCase) && fileName != nullptr)
        {
            try
            {

                String^ record_time = fileName;
                int number = Convert::ToInt32(record_time);
                if (!(number > 0 && number <= 10))
                    number = 10;

                record_time = Convert::ToString(number);

                Process::Start("ffmpeg", "-list_devices true -f dshow -i dummy");

                // Record video using Process
                Process^ ffmpegProcess = gcnew Process();
                ffmpegProcess->StartInfo->FileName = "ffmpeg";
                String^ command_1 = "-f dshow -video_size 640x480 -framerate 30 -rtbufsize 100M -i video=\"HD Webcam\" -t ";
                String^ command_2 = " -vcodec h264 -preset ultrafast -pix_fmt yuv420p -y output.mp4";
                String^ main_command = command_1 + record_time + command_2;
                ffmpegProcess->StartInfo->Arguments = main_command;
                ffmpegProcess->StartInfo->UseShellExecute = false;
                ffmpegProcess->Start();
                ffmpegProcess->WaitForExit();

                String^ videoPath = "output.mp4";
                array<Byte>^ videoData = File::ReadAllBytes(videoPath);
                array<Byte>^ sizeBuffer = BitConverter::GetBytes(videoData->Length);

                // Gửi kích thước trước
                stream->Write(sizeBuffer, 0, sizeBuffer->Length);
                stream->Flush();

                // Gửi toàn bộ video
                stream->Write(videoData, 0, videoData->Length);
                stream->Flush();

                form->UpdateCommunicationLog("Video sent to client.");
            }
            catch (Exception^ ex)
            {
                form->UpdateCommunicationLog("Error sending video: " + ex->Message);
            }
  
        }
        else if (command->Equals("START_RECORDING", StringComparison::OrdinalIgnoreCase))
        {
            // Đường dẫn tương đối lưu file ghi âm
            String^ relativePath = Application::StartupPath + "\\record.wav";

            Process^ recorder = gcnew Process();
            recorder->StartInfo->FileName = "powershell";
            recorder->StartInfo->Arguments = "-Command \"Add-Type -TypeDefinition 'using System; using System.Runtime.InteropServices; namespace SoundRecording { public class Audio { [DllImport(\\\"winmm.dll\\\")] public static extern int mciSendString(string command, string buffer, int bufferSize, IntPtr hwndCallback); } }'; [SoundRecording.Audio]::mciSendString('open new Type waveaudio Alias recsound', $null, 0, [IntPtr]::Zero); [SoundRecording.Audio]::mciSendString('record recsound', $null, 0, [IntPtr]::Zero); Start-Sleep -Seconds 60; [SoundRecording.Audio]::mciSendString('save recsound \"" + relativePath + "\"', $null, 0, [IntPtr]::Zero); [SoundRecording.Audio]::mciSendString('close recsound', $null, 0, [IntPtr]::Zero)\"";
            recorder->StartInfo->UseShellExecute = false;
            recorder->StartInfo->RedirectStandardOutput = true;
            recorder->Start();

            form->UpdateCommunicationLog("Recording started for 1 minute and saved as '" + relativePath + "'.");
            }
        else if (action->Equals("START_APP", StringComparison::OrdinalIgnoreCase) && fileName != nullptr)
        {
            try
            {
                ProcessStartInfo^ startInfo = gcnew ProcessStartInfo();
                startInfo->FileName = fileName;       // Dùng fileName truyền vào
                startInfo->UseShellExecute = true;    // Cho phép mở ứng dụng có giao diện
                Process::Start(startInfo);

                form->UpdateCommunicationLog("Started application: " + fileName);

                // Gửi phản hồi thành công (byte 1)
                array<Byte>^ successResponse = gcnew array<Byte>(1) { 1 };
                stream->Write(successResponse, 0, successResponse->Length);
            }
            catch (Exception^ ex)
            {
                form->UpdateCommunicationLog("Failed to start application: " + fileName + "\nError: " + ex->Message);

                // Gửi phản hồi thất bại (byte 0)
                array<Byte>^ failureResponse = gcnew array<Byte>(1) { 0 };
                stream->Write(failureResponse, 0, failureResponse->Length);
            }
            stream->Flush();
        }


        else if (action->Equals("STOP_APP", StringComparison::OrdinalIgnoreCase) && fileName != nullptr)
        {
            try
            {
                // Lấy tên file từ đường dẫn
                String^ processName = Path::GetFileName(fileName); // Chỉ lấy oald8.exe

                Process^ killProcess = gcnew Process();
                killProcess->StartInfo->FileName = "taskkill";
                killProcess->StartInfo->Arguments = "/IM \"" + processName + "\" /F"; // Dùng tên file, không dùng đường dẫn
                killProcess->StartInfo->UseShellExecute = false;
                killProcess->StartInfo->RedirectStandardOutput = true;
                killProcess->StartInfo->RedirectStandardError = true;
                killProcess->Start();

                // Đọc kết quả và lỗi
                String^ output = killProcess->StandardOutput->ReadToEnd();
                String^ error = killProcess->StandardError->ReadToEnd();
                killProcess->WaitForExit();

                if (killProcess->ExitCode == 0)
                {
                    form->UpdateCommunicationLog("Application stopped successfully: " + processName + "\nOutput: " + output);

                    array<Byte>^ successResponse = gcnew array<Byte>(1) { 1 };
                    stream->Write(successResponse, 0, successResponse->Length);
                }
                else
                {
                    form->UpdateCommunicationLog("Failed to stop application: " + processName + "\nError: " + error);

                    array<Byte>^ failureResponse = gcnew array<Byte>(1) { 0 };
                    stream->Write(failureResponse, 0, failureResponse->Length);
                }
                stream->Flush();
            }
            catch (Exception^ ex)
            {
                form->UpdateCommunicationLog("Exception occurred while stopping application: " + ex->Message);

                array<Byte>^ failureResponse = gcnew array<Byte>(1) { 0 };
                stream->Write(failureResponse, 0, failureResponse->Length);
                stream->Flush();
            }

        }
        else if (action->Equals("START_SERVICE", StringComparison::OrdinalIgnoreCase) && fileName != nullptr)
        {
            Process^ startServiceProcess = gcnew Process();
            startServiceProcess->StartInfo->FileName = "sc";
            startServiceProcess->StartInfo->Arguments = "start \"" + fileName + "\"";
            startServiceProcess->StartInfo->UseShellExecute = false;
            startServiceProcess->StartInfo->RedirectStandardOutput = true;
            startServiceProcess->StartInfo->RedirectStandardError = true; // Đọc lỗi nếu có
            startServiceProcess->Start();

            // Đọc kết quả đầu ra và lỗi
            String^ output = startServiceProcess->StandardOutput->ReadToEnd();
            String^ error = startServiceProcess->StandardError->ReadToEnd();
            startServiceProcess->WaitForExit();

            // Kiểm tra kết quả trả về từ lệnh
            if (startServiceProcess->ExitCode == 0) // ExitCode = 0 là thành công
            {
                form->UpdateCommunicationLog("Service started successfully: " + fileName);

                // Gửi phản hồi thành công (byte 1)
                array<Byte>^ successResponse = gcnew array<Byte>(1) { 1 };
                stream->Write(successResponse, 0, successResponse->Length);
            }
            else
            {
                form->UpdateCommunicationLog("Failed to start service: " + fileName + "\nError: " + error);

                // Gửi phản hồi thất bại (byte 0)
                array<Byte>^ failureResponse = gcnew array<Byte>(1) { 0 };
                stream->Write(failureResponse, 0, failureResponse->Length);
            }
            stream->Flush(); // Đảm bảo gửi dữ liệu ngay lập tức
        }
        else if (action->Equals("STOP_SERVICE", StringComparison::OrdinalIgnoreCase) && fileName != nullptr)
        {
            Process^ stopServiceProcess = gcnew Process();
            stopServiceProcess->StartInfo->FileName = "sc";
            stopServiceProcess->StartInfo->Arguments = "stop \"" + fileName + "\"";
            stopServiceProcess->StartInfo->UseShellExecute = false;
            stopServiceProcess->StartInfo->RedirectStandardOutput = true;
            stopServiceProcess->StartInfo->RedirectStandardError = true; // Đọc lỗi nếu có
            stopServiceProcess->Start();

            // Đọc kết quả đầu ra và chờ quá trình hoàn thành
            String^ output = stopServiceProcess->StandardOutput->ReadToEnd();
            String^ error = stopServiceProcess->StandardError->ReadToEnd();
            stopServiceProcess->WaitForExit();

            // Kiểm tra kết quả trả về từ lệnh
            if (stopServiceProcess->ExitCode == 0) // ExitCode = 0 thường là thành công
            {
                form->UpdateCommunicationLog("Service stopped successfully: " + fileName);

                // Gửi phản hồi thành công (byte 1)
                array<Byte>^ successResponse = gcnew array<Byte>(1) { 1 };
                stream->Write(successResponse, 0, successResponse->Length);
            }
            else
            {
                form->UpdateCommunicationLog("Failed to stop service: " + fileName + "\nError: " + error);

                // Gửi phản hồi thất bại (byte 0)
                array<Byte>^ failureResponse = gcnew array<Byte>(1) { 0 };
                stream->Write(failureResponse, 0, failureResponse->Length);
            }
            stream->Flush(); // Đảm bảo gửi dữ liệu ngay lập tức
        }

        else if (action->Equals("GET_FILE", StringComparison::OrdinalIgnoreCase) && fileName != nullptr)
        {
            if (System::IO::File::Exists(fileName))
            {
                try
                {
                    // Đọc dữ liệu từ file
                    array<Byte>^ fileData = System::IO::File::ReadAllBytes(fileName);

                    // Chuyển đổi tên file sang UTF-8 để truyền đúng ký tự đặc biệt và tiếng Việt
                    array<Byte>^ fileNameBytes = Encoding::UTF8->GetBytes(fileName);
                    int fileNameLength = fileNameBytes->Length;

                    // Gửi độ dài của tên file (4 byte)
                    array<Byte>^ nameLengthBuffer = BitConverter::GetBytes(fileNameLength);
                    stream->Write(nameLengthBuffer, 0, nameLengthBuffer->Length);
                    stream->Flush(); // Đảm bảo dữ liệu đã được gửi

                    // Gửi tên file
                    stream->Write(fileNameBytes, 0, fileNameBytes->Length);
                    stream->Flush(); // Đảm bảo tên file đã được gửi

                    // Gửi kích thước của file (4 byte)
                    array<Byte>^ sizeBuffer = BitConverter::GetBytes(fileData->Length);
                    stream->Write(sizeBuffer, 0, sizeBuffer->Length);
                    stream->Flush(); // Đảm bảo kích thước đã được gửi

                    // Gửi nội dung file
                    stream->Write(fileData, 0, fileData->Length);
                    stream->Flush(); // Đảm bảo dữ liệu file đã được gửi

                    form->UpdateCommunicationLog("File " + fileName + " sent to client.");
                }
                catch (Exception^ ex)
                {
                    form->UpdateCommunicationLog("Error sending file: " + ex->Message);
                }
            }
            else
            {
                // Nếu không tìm thấy file
                form->UpdateCommunicationLog("File not found: " + fileName);
                String^ errorMessage = "ERROR: File not found";
                array<Byte>^ errorData = Encoding::UTF8->GetBytes(errorMessage);
                stream->Write(errorData, 0, errorData->Length);
                stream->Flush(); // Đảm bảo thông báo lỗi đã được gửi
            }
            }

        else if (action->Equals("DELETE_FILE", StringComparison::OrdinalIgnoreCase) && fileName != nullptr)
        {
            // Xóa file
            if (System::IO::File::Exists(fileName))
            {
                System::IO::File::Delete(fileName);
                form->UpdateCommunicationLog("File deleted: " + fileName);
                array<Byte>^ successResponse = gcnew array<Byte>(1) { 1 };
                stream->Write(successResponse, 0, successResponse->Length);
                stream->Flush();
            }
            else
            {
                form->UpdateCommunicationLog("File not found: " + fileName);
                array<Byte>^ failureResponse = gcnew array<Byte>(1) { 0 };
                stream->Write(failureResponse, 0, failureResponse->Length);
                stream->Flush();
            }
        }
    }

    void ServerForm::checkBoxDarkMode_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
    {
        if (this->checkBoxDarkMode->Checked) // Chế độ tối
        {
            this->BackColor = System::Drawing::Color::FromArgb(45, 45, 48);                      // Màu nền tối
            this->textBoxServerOutput->BackColor = System::Drawing::Color::FromArgb(30, 30, 30); // Màu nền hộp text
            this->textBoxServerOutput->ForeColor = System::Drawing::Color::White;                // Màu chữ
            this->buttonStartServer->BackColor = System::Drawing::Color::DarkOliveGreen;         // Màu nút
            this->buttonStopServer->BackColor = System::Drawing::Color::Firebrick;
            this->labelStatus->ForeColor = System::Drawing::Color::White; // Màu chữ trạng thái
        }
        else // Chế độ sáng
        {
            this->BackColor = System::Drawing::Color::White; // Màu nền sáng
            this->textBoxServerOutput->BackColor = System::Drawing::Color::AliceBlue;
            this->textBoxServerOutput->ForeColor = System::Drawing::Color::Black;
            this->buttonStartServer->BackColor = System::Drawing::Color::LightGreen;
            this->buttonStopServer->BackColor = System::Drawing::Color::Tomato;
            this->labelStatus->ForeColor = System::Drawing::Color::Gray; // Màu chữ mặc định
        }
    }

    // Hàm khởi động server
    System::Void ServerForm::buttonStartServer_Click(System::Object^ sender, System::EventArgs^ e)
    {

        this->labelStatus->Text = "Status: Server is running";
        this->UpdateCommunicationLog("Server started...");
        // progressBarServerStatus->Value = 50; // cập nhật giá trị khi server đang khởi động
        pictureBoxStatus->BackColor = System::Drawing::Color::LightGreen;

        Thread^ serverThread = gcnew Thread(gcnew ThreadStart(this, &ServerForm::StartServer));
        serverThread->IsBackground = true; // Đảm bảo luồng này kết thúc khi ứng dụng chính đóng
        serverThread->Start();
    }
    void ServerForm::StartServer()
    {

        TcpListener^ listener = gcnew TcpListener(IPAddress::Any, 12345); // Lắng nghe trên tất cả các địa chỉ IP

        listener->Start();

        this->UpdateCommunicationLog("Server is waiting for commands...");

        while (true)
        {
            TcpClient^ client = listener->AcceptTcpClient();
            IPEndPoint^ clientEndPoint = dynamic_cast<IPEndPoint^>(client->Client->RemoteEndPoint);

            // Display the client's IP address and port
            String^ clientInfo = "Client connected from: " + clientEndPoint->Address->ToString() +
                ":" + clientEndPoint->Port.ToString();
            this->UpdateCommunicationLog(clientInfo);

            NetworkStream^ stream = client->GetStream();
            array<Byte>^ passwordBuffer = gcnew array<Byte>(256);
            int passwordBytes = stream->Read(passwordBuffer, 0, passwordBuffer->Length);
            String^ receivedPassword = Encoding::UTF8->GetString(passwordBuffer, 0, passwordBytes)->Trim();
            this->UpdateCommunicationLog("Expected password: " + User::Password);


            if (receivedPassword != User::Password)
            {
                this->UpdateCommunicationLog("Client sent incorrect password. Connection rejected.");

                // Gửi phản hồi số 0 (mật khẩu sai)
                array<Byte>^ response = gcnew array<Byte>(1);
                response[0] = false; // 0 cho mật khẩu sai
                stream->Write(response, 0, response->Length);

                // Đóng kết nối
                stream->Close();
                client->Close();
                continue; // Quay lại chấp nhận kết nối mới
            }
            array<Byte>^ successResponse = gcnew array<Byte>(1);
            successResponse[0] = true; // 1 cho mật khẩu đúng
            stream->Write(successResponse, 0, successResponse->Length);

            this->UpdateCommunicationLog("Password verified. Connection accepted.");


            array<Byte>^ data = gcnew array<Byte>(256);
            int bytes = stream->Read(data, 0, data->Length);
            String^ command = Encoding::UTF8->GetString(data, 0, bytes);
            command = command->Trim();

            this->UpdateCommunicationLog("Received command: " + command);
            ExecuteCommand(command, this, stream); // Gọi hàm ExecuteCommand

            stream->Close();
            client->Close(); 
        }

        listener->Stop();
    }

    // Hàm dừng server
    System::Void ServerForm::buttonStopServer_Click(System::Object^ sender, System::EventArgs^ e)
    {
        this->labelStatus->Text = "Status: Server is stopped";
        pictureBoxStatus->BackColor = System::Drawing::Color::Tomato;
        this->UpdateCommunicationLog("Server stopped...");
    }
}