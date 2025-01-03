#include "Client.h"
#include "ServerForm.h"
using namespace System;
using namespace System::Globalization;
using namespace System::IO;
using namespace System::Threading;
using namespace System::Net;
using namespace System::Net::Mail;
using namespace System::Net::Sockets;
using namespace System::Text;
using namespace EAGetMail; // Thư viện EAGetMail cho email
using namespace System::Windows::Forms;

// Hàm tạo tên file tạm thời
System::Void LoginForm::Client::switchToServerButton_Click(System::Object^ sender, System::EventArgs^ e) {
    this->Hide(); // Hide the current Client form
    ServerForm^ serverForm = gcnew ServerForm();
    serverForm->Show();
}

void LoginForm::Client::StartApplication(String^ emailAddress, String^ appPassword, String^ specificSender, int checkInterval) {
    RunClient(emailAddress, appPassword, specificSender, checkInterval);
}

// Hàm tạo tên file tạm thời
String^ LoginForm::Client::GenerateFileName(int sequence) {
    DateTime currentDateTime = DateTime::Now;
    return String::Format("{0}-{1:000}-{2:000}.txt",
        currentDateTime.ToString("yyyyMMddHHmmss", gcnew CultureInfo("en-US")),
        currentDateTime.Millisecond,
        sequence);
}

// Hàm tìm thư mục Inbox
Imap4Folder^ LoginForm::Client::FindInboxFolder(array<Imap4Folder^>^ folders) {
    for each (Imap4Folder ^ folder in folders) {
        if (folder->Name->Equals("INBOX", StringComparison::OrdinalIgnoreCase)) {
            return folder;
        }
    }
    return nullptr;
}

// Hàm kiểm tra xem email đã được xử lý chưa
bool LoginForm::Client::IsEmailProcessed(String^ messageId, String^ processedEmailsFile) {
    if (!File::Exists(processedEmailsFile)) {
        return false; // Nếu file chưa tồn tại, email chưa được xử lý
    }

    array<String^>^ processedEmails = File::ReadAllLines(processedEmailsFile);
    for each (String ^ id in processedEmails) {
        if (id->Equals(messageId)) {
            return true; // Email đã được xử lý
        }
    }
    return false;
}

// Hàm lưu lại email đã xử lý
void LoginForm::Client::SaveProcessedEmail(String^ messageId, String^ processedEmailsFile) {
    File::AppendAllText(processedEmailsFile, messageId + Environment::NewLine);
}

// Kiểm tra xem email có phải từ người gửi cụ thể không
bool LoginForm::Client::IsEmailFromSpecificSender(EAGetMail::Mail^ oMail, String^ specificSender) {
    return oMail->From->Address->Equals(specificSender, StringComparison::OrdinalIgnoreCase);
}

// Nhận hình ảnh từ stream
void LoginForm::Client::ReceiveImage(NetworkStream^ stream) {
    array<Byte>^ sizeBuffer = gcnew array<Byte>(4);
    stream->Read(sizeBuffer, 0, sizeBuffer->Length);
    int imageSize = BitConverter::ToInt32(sizeBuffer, 0);
    array<Byte>^ imageData = gcnew array<Byte>(imageSize);
    stream->Read(imageData, 0, imageData->Length);
    String^ imagePath = "screenshot_" + DateTime::Now.ToString("yyyyMMdd_HHmmss") + ".png";
    File::WriteAllBytes(imagePath, imageData);
    AppendLog("Image received and saved as: " + imagePath);
}

// Hàm gửi email
void LoginForm::Client::SendEmail(String^ recipient, String^ subject, String^ body, String^ attachmentPath, String^Credent, String^ app_password) {
    try {
        if (String::IsNullOrEmpty(recipient) || !System::Text::RegularExpressions::Regex::IsMatch(recipient, "^\\S+@\\S+\\.\\S+$")) {
            throw gcnew ArgumentException("Địa chỉ email không hợp lệ: " + recipient);
        }

        if (String::IsNullOrEmpty(subject)) {
            subject = "No Subject";
        }
        if (String::IsNullOrEmpty(body)) {
            body = "No Content";
        }

        SmtpClient^ smtpClient = gcnew SmtpClient("smtp.gmail.com", 587);
        smtpClient->Credentials = gcnew NetworkCredential(Credent, app_password);
        smtpClient->EnableSsl = true;

        MailMessage^ mailMessage = gcnew MailMessage();
        mailMessage->From = gcnew System::Net::Mail::MailAddress(Credent);
        mailMessage->To->Add(recipient);
        mailMessage->Subject = subject;
        mailMessage->Body = body;

        if (!String::IsNullOrEmpty(attachmentPath)) {
            if (System::IO::File::Exists(attachmentPath)) {
                mailMessage->Attachments->Add(gcnew System::Net::Mail::Attachment(attachmentPath));
            }
        }

        smtpClient->Send(mailMessage);
        AppendLog("Email sent successfully to " + recipient);
    }
    catch (Exception^ ex) {
        AppendLog("Error sending email: " + ex->Message);
        AppendLog("Stack Trace: " + ex->StackTrace);
    }
}

array<String^>^ LoginForm::Client::ExtractEmailDetails(String^ emailBody) {
    if (String::IsNullOrEmpty(emailBody)) {
        return nullptr; // Không có nội dung email
    }

    // Tách các dòng trong nội dung email
    array<String^>^ lines = emailBody->Split(gcnew array<wchar_t>{'\r', '\n'}, StringSplitOptions::RemoveEmptyEntries);

    if (lines->Length < 2) {
        return nullptr; // Không đủ dòng để lấy IP và mật khẩu
    }

    // Phần tử đầu tiên là địa chỉ IP
    String^ ip = lines[0]->Trim();

    // Phần tử thứ hai là mật khẩu
    String^ password = lines[1]->Trim();

    // Các dòng còn lại là lệnh
    array<String^>^ commands = gcnew array<String^>(lines->Length - 2);
    for (int i = 2; i < lines->Length; i++) {
        commands[i - 2] = lines[i]->Trim();
    }

    // Gộp tất cả vào một mảng kết quả
    array<String^>^ result = gcnew array<String^>(lines->Length);
    result[0] = ip;
    result[1] = password;
    for (int i = 0; i < commands->Length; i++) {
        result[i + 2] = commands[i];
    }

    return result;
}

// Xử lý phản hồi từ server
void LoginForm::Client::handleResponse(String^ command, NetworkStream^ stream, String^ recipientEmail, String^ Credent, String^ app_password) {
    if (command->Equals("TAKE_SCREENSHOT", StringComparison::OrdinalIgnoreCase) ||
        command->Equals("TAKE_PHOTO", StringComparison::OrdinalIgnoreCase) ||
        command->Contains("SEND_VIDEO")) {

        array<Byte>^ sizeBuffer = gcnew array<Byte>(4);
        int bytesRead = stream->Read(sizeBuffer, 0, sizeBuffer->Length);
        if (bytesRead != sizeBuffer->Length) {
            AppendLog("Failed to receive image/video size.");
            return;
        }

        int dataSize = BitConverter::ToInt32(sizeBuffer, 0);
        if (dataSize <= 0) {
            AppendLog("Invalid data size received.");
            return;
        }

        array<Byte>^ data = gcnew array<Byte>(dataSize);
        int totalBytesRead = 0;
        while (totalBytesRead < dataSize) {
            int read = stream->Read(data, totalBytesRead, dataSize - totalBytesRead);
            if (read <= 0) {
                AppendLog("Failed to receive complete data.");
                return;
            }
            totalBytesRead += read;
        }

        String^ filePath = (command->Contains("SEND_VIDEO") ? "Video_" : "Image_") + DateTime::Now.ToString("yyyyMMdd_HHmmss") +
            (command->Contains("SEND_VIDEO") ? ".mp4" : ".png");
        File::WriteAllBytes(filePath, data);
        AppendLog("Data received and saved as: " + filePath);

        String^ subject = (command->Contains("SEND_VIDEO") ? "Video" : "Image") + " from Server";
        String^ body = "Here is the " + (command->Contains("SEND_VIDEO") ? "video" : "image") + " you requested.";
        SendEmail(recipientEmail, subject, body, filePath, Credent, app_password);
    }
    else if (command->Contains("GET_FILE")) {
        AppendLog("Requesting file from server...");

        try {
            // Đọc độ dài của tên file
            array<Byte>^ nameLengthBuffer = gcnew array<Byte>(4);
            int bytesRead = stream->Read(nameLengthBuffer, 0, nameLengthBuffer->Length);
            if (bytesRead != nameLengthBuffer->Length) {
                AppendLog("Failed to receive file name length.");
                return;
            }

            int nameLength = BitConverter::ToInt32(nameLengthBuffer, 0);
            if (nameLength <= 0) {
                AppendLog("Invalid file name length received.");
                return;
            }

            // Đọc tên file (chứa ký tự đặc biệt và tiếng Việt)
            array<Byte>^ nameBuffer = gcnew array<Byte>(nameLength);
            bytesRead = stream->Read(nameBuffer, 0, nameBuffer->Length);
            if (bytesRead != nameBuffer->Length) {
                AppendLog("Failed to receive file name.");
                return;
            }

            // Chuyển byte thành chuỗi UTF-8 để đảm bảo ký tự đặc biệt được đọc đúng
            String^ originalFilePath = Encoding::UTF8->GetString(nameBuffer);
            String^ fileName = Path::GetFileName(originalFilePath); // Trích xuất tên file
            AppendLog("Receiving file: " + fileName);

            // Đọc kích thước file
            array<Byte>^ sizeBuffer = gcnew array<Byte>(4);
            bytesRead = stream->Read(sizeBuffer, 0, sizeBuffer->Length);
            if (bytesRead != sizeBuffer->Length) {
                AppendLog("Failed to receive file size.");
                return;
            }

            int fileSize = BitConverter::ToInt32(sizeBuffer, 0);
            if (fileSize <= 0) {
                AppendLog("Invalid file size received.");
                return;
            }

            // Đọc dữ liệu file
            array<Byte>^ fileData = gcnew array<Byte>(fileSize);
            int totalBytesRead = 0;
            while (totalBytesRead < fileSize) {
                int read = stream->Read(fileData, totalBytesRead, fileSize - totalBytesRead);
                if (read <= 0) {
                    AppendLog("Failed to receive complete file data.");
                    return;
                }
                totalBytesRead += read;
            }

            // Lưu file với tên mới (trích xuất từ file path gốc)
            File::WriteAllBytes(fileName, fileData);
            AppendLog("File received and saved as: " + fileName);

            // Gửi email với file đính kèm
            String^ subject = "File Received from Server";
            String^ body = "Here is the file you requested.";
            SendEmail(recipientEmail, subject, body, fileName, Credent, app_password);
            AppendLog("File sent via email: " + fileName);
        }
        catch (Exception^ ex) {
            AppendLog("Error while receiving file: " + ex->Message);
        }
    }
    else if (command->Contains("DELETE_FILE")) {
        array<Byte>^ buffer = gcnew array<Byte>(1); // Chỉ cần đọc 1 byte
        int bytesRead = stream->Read(buffer, 0, buffer->Length);

        if (bytesRead > 0) {
            // Kiểm tra xem byte đọc được có phải là 1 hay 0
            if (buffer[0] == 1) {
                String^ subject = "Response from Server";
                String^ body = "The file you requested has been deleted.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("File deleted successfully. Email sent.");
            }
            else if (buffer[0] == 0) {
                String^ subject = "Response from Server";
                String^ body = "There was an error deleting the file. Please try again.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Error deleting file. Email sent.");
            }
        }
        else {
            String^ subject = "Response from Server";
            String^ body = "There seems to be an error. Please try again.";
            SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
            AppendLog("No response from server.");
        }
    }
    else if (command->Contains("START_SERVICE")) {
        array<Byte>^ buffer = gcnew array<Byte>(1); // Chỉ cần đọc 1 byte
        int bytesRead = stream->Read(buffer, 0, buffer->Length);

        if (bytesRead > 0) {
            // Kiểm tra xem byte đọc được có phải là 1 hay 0
            if (buffer[0] == 1) {
                String^ subject = "Response from Server";
                String^ body = "The service you requested has been started.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Service started successfully. Email sent.");
            }
            else if (buffer[0] == 0) {
                String^ subject = "Response from Server";
                String^ body = "There was an error starting the service. Please try again.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Error starting service. Email sent.");
            }
        }
        else {
            String^ subject = "Response from Server";
            String^ body = "There seems to be an error. Please try again.";
            SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
            AppendLog("No response from server.");
        }
    }
    else if (command->Contains("STOP_SERVICE")) {
        array<Byte>^ buffer = gcnew array<Byte>(1); // Chỉ cần đọc 1 byte
        int bytesRead = stream->Read(buffer, 0, buffer->Length);

        if (bytesRead > 0) {
            // Kiểm tra xem byte đọc được có phải là 1 hay 0
            if (buffer[0] == 1) {
                String^ subject = "Response from Server";
                String^ body = "The service you requested has been stopped.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Service stopped successfully. Email sent.");
            }
            else if (buffer[0] == 0) {
                String^ subject = "Response from Server";
                String^ body = "There was an error stopping the service. Please try again.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Error stopping service. Email sent.");
            }
        }
        else {
            String^ subject = "Response from Server";
            String^ body = "There seems to be an error. Please try again.";
            SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
            AppendLog("No response from server.");
        }
    }
    else if (command->Contains("SHUTDOWN")) {
        array<Byte>^ buffer = gcnew array<Byte>(1); // Chỉ cần đọc 1 byte
        int bytesRead = stream->Read(buffer, 0, buffer->Length);
        if (bytesRead > 0) {
            // Kiểm tra xem byte đọc được có phải là 1 hay 0
            if (buffer[0] == 1) {
                String^ subject = "Response from Server";
                String^ body = "The server is shutting down.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Server shut down successfully. Email sent.");
            }
            else if (buffer[0] == 0) {
                String^ subject = "Response from Server";
                String^ body = "There was an error shutting down the server. Please try again.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Error shutting down the server. Email sent.");
            }
        }
        else {
            String^ subject = "Response from Server";
            String^ body = "There seems to be an error. Please try again.";
            SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
            AppendLog("No response from server.");
        }
    }
    else if (command->Contains("START_APP")) {
        array<Byte>^ buffer = gcnew array<Byte>(1); // Chỉ cần đọc 1 byte
        int bytesRead = stream->Read(buffer, 0, buffer->Length);

        if (bytesRead > 0)
        {
            // Kiểm tra xem byte đọc được có phải là 1 hay 0
            if (buffer[0] == 1)
            {
                String^ subject = "Response from Server";
                String^ body = "The application you requested has been started.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Application started successfully. Email sent.");
            }
            else if (buffer[0] == 0)
            {
                String^ subject = "Response from Server";
                String^ body = "There was an error starting the application. Please try again.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Error starting application. Email sent.");
            }
        }
        else
        {
            String^ subject = "Response from Server";
            String^ body = "There seems to be an error. Please try again.";
            SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
            AppendLog("No response from server.");
        }
    }
    else if (command->Contains("STOP_APP")) {
        array<Byte>^ buffer = gcnew array<Byte>(1); // Chỉ cần đọc 1 byte
        int bytesRead = stream->Read(buffer, 0, buffer->Length);

        if (bytesRead > 0) {
            // Kiểm tra xem byte đọc được có phải là 1 hay 0
            if (buffer[0] == 1) {
                String^ subject = "Response from Server";
                String^ body = "The application you requested has been stopped.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Application stopped successfully. Email sent.");
            }
            else if (buffer[0] == 0) {
                String^ subject = "Response from Server";
                String^ body = "There was an error stopping the application. Please try again.";
                SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
                AppendLog("Error stopping application. Email sent.");
            }
        }
        else {
            String^ subject = "Response from Server";
            String^ body = "There seems to be an error. Please try again.";
            SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
            AppendLog("No response from server.");
        }
    }
    else if (command->Contains("LIST_APPS")) {
        array<Byte>^ buffer = gcnew array<Byte>(65536);
        int bytesRead = stream->Read(buffer, 0, buffer->Length);
        if (bytesRead > 0) {
            String^ response = Encoding::UTF8->GetString(buffer, 0, bytesRead);
            AppendLog("Response from server:\r\n{0}" + response);

            String^ filePath = "response_" + DateTime::Now.ToString("yyyyMMdd_HHmmss") + ".txt";
            File::WriteAllText(filePath, response);
            String^ subject = "Response from Server";
            String^ body = "Here is the list of running apps saved in a text file.";
            SendEmail(recipientEmail, subject, body, filePath, Credent, app_password);
            AppendLog("Response saved to file and sent via email.");
        }
        else {
            String^ subject = "Response from Server";
            String^ body = "There seems to be an error. Please try again.";
            SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
            AppendLog("No response from server.");
        }
    }
    else if (command->Contains("LIST_SERVICES")) {
        array<Byte>^ buffer = gcnew array<Byte>(65536);
        int bytesRead = stream->Read(buffer, 0, buffer->Length);
        if (bytesRead > 0) {
            String^ response = Encoding::UTF8->GetString(buffer, 0, bytesRead);
            AppendLog("Response from server:\r\n{0}"+ response);

            String^ filePath = "response_" + DateTime::Now.ToString("yyyyMMdd_HHmmss") + ".txt";
            File::WriteAllText(filePath, response);
            String^ subject = "Response from Server";
            String^ body = "Here is the list of running services saved in a text file.";
            SendEmail(recipientEmail, subject, body, filePath, Credent, app_password);
            AppendLog("Response saved to file and sent via email.");
        }
        else {
            String^ subject = "Response from Server";
            String^ body = "There seems to be an error. Please try again.";
            SendEmail(recipientEmail, subject, body, nullptr, Credent, app_password);
            AppendLog("No response from server.");
        }
    }
}
bool LoginForm::Client::HandleVerification(NetworkStream^ stream) {
    try {
        if (stream == nullptr || !stream->CanRead) {
            AppendLog("Stream is null or cannot be read.");
            return false; // Không thể đọc từ luồng
        }

        // Tạo bộ đệm để nhận dữ liệu
        array<Byte>^ buffer = gcnew array<Byte>(1); // Chỉ cần 1 byte để nhận phản hồi
        int bytesRead = stream->Read(buffer, 0, buffer->Length);

        if (bytesRead == 0) {
            AppendLog("No data received from server.");
            return false; // Không nhận được dữ liệu
        }

        // Phân tích byte nhận được
        bool response = buffer[0] != 0; // 0 tương ứng false, ngược lại là true
        if (response) {
            AppendLog("Password verification succeeded.");
            return true; // Mật khẩu chính xác
        }
        else {
            AppendLog("Password verification failed.");
            return false; // Mật khẩu không chính xác
        }
    }
    catch (Exception^ ex) {
        AppendLog("Error during verification handling: " + ex->Message);
        return false; // Có lỗi xảy ra
    }
}
// Hàm chạy client với các tham số truyền vào
void LoginForm::Client::RunClient(String^ emailAddress, String^ appPassword, String^ specificSender, int checkInterval) {
    try {
        DateTime startTime = DateTime::Now;
        String^ processedEmailsFile = Path::Combine(Directory::GetCurrentDirectory(), "processed_emails.txt");

        MailServer^ oServer = gcnew MailServer("imap.gmail.com", emailAddress, appPassword, ServerProtocol::Imap4);
        oServer->SSLConnection = true;
        oServer->Port = 993;

        AppendLog("Connecting to IMAP server...");

        MailClient^ oClient = gcnew MailClient("TryIt");
        if (File::Exists(processedEmailsFile)) {
            File::Delete(processedEmailsFile);
      
        }

        while (true) {
            try {
                oClient->Connect(oServer);
  
                array<Imap4Folder^>^ folders = oClient->GetFolders();
                Imap4Folder^ inboxFolder = FindInboxFolder(folders);

                if (inboxFolder != nullptr) {
                    oClient->SelectFolder(inboxFolder);
                    array<MailInfo^>^ infos = oClient->GetMailInfos();
         

                    if (infos->Length > 0) {
                        for (int i = infos->Length - 1; i >= 0; i--) {
                            MailInfo^ mailInfo = infos[i];
                            String^ messageId = mailInfo->UIDL;

                            if (IsEmailProcessed(messageId, processedEmailsFile)) {
                     
                                continue;
                            }

                            EAGetMail::Mail^ oMail = oClient->GetMail(mailInfo);
                            DateTime sentTime = oMail->ReceivedDate;

                            if (sentTime < startTime) {
                                break;
                            }

                            AppendLog("Processing email sent at: {0}"+ sentTime);

                            if (specificSender->Length == 0 || IsEmailFromSpecificSender(oMail, specificSender)) {
                                AppendLog("From: {0}" + oMail->From->ToString());
                                AppendLog("Subject: {0}\r\n"+ oMail->Subject);
                                AppendLog("Body: {0}\r\n"+oMail->TextBody);

                                array<String^>^ details = ExtractEmailDetails(oMail->TextBody->Trim());
                                if (details == nullptr) {
                                    String^ subject = "Error: The message is empty";
                                    String^ body = "The message is empty, please send again";
                                    SendEmail(oMail->From->Address, subject, body, nullptr, emailAddress, appPassword);
                                    oClient->MarkAsRead(mailInfo, true);
                                    SaveProcessedEmail(messageId, processedEmailsFile);
                                    continue;
                                }
                                if (details->Length < 3) {
                                    String^ subject = "Error: The message is misinformation";
                                    String^ body = "Maybe you miss IP address, password or command, please send enough 3 lines";
                                    SendEmail(oMail->From->Address, subject, body, nullptr, emailAddress, appPassword);
                                    oClient->MarkAsRead(mailInfo, true);
                                    SaveProcessedEmail(messageId, processedEmailsFile);
                                    continue;
                                }
                                String^ ip_add = details[0];
                                String^ pass = details[1];
                                

                                String^ command = details[2];

                                try {
                                    TcpClient^ client = gcnew TcpClient();
                                    client->Connect(ip_add, 12345);
                                    NetworkStream^ stream = client->GetStream();
                                    array<Byte>^ passVerified = Encoding::UTF8->GetBytes(pass);
                                    stream->Write(passVerified, 0, passVerified->Length);
                                    stream->Flush();
                                    bool verification = HandleVerification(stream);
                                    if (verification == false) {
                                        String^ subject = "Error: The password is wrong";
                                        String^ body = "Server rejected the connection due to an incorrect password, please try again";
                                        SendEmail(oMail->From->Address, subject, body, nullptr, emailAddress, appPassword);
                                        continue;
                                    }
                                    array<Byte>^ data = Encoding::UTF8->GetBytes(command);
                                    stream->Write(data, 0, data->Length);
                                    stream->Flush();

                                    handleResponse(command, stream, specificSender, emailAddress, appPassword);
                                    stream->Close();
                                    client->Close();
                                }
                                catch (Exception^ ex) {
                                    AppendLog("Error connecting to server at IP {0}: {1}"+ ip_add + ex->Message);
                                    String^ subject = "Error: Failed to Connect to Server";
                                    String^ body = "The IP address provided in your email (" + ip_add +
                                        ") could not be connected to. Please verify the IP address and try again.\r\n\r\nError Details:\r\n" + ex->Message;
                                    SendEmail(oMail->From->Address, subject, body, nullptr, emailAddress, appPassword);
                                }

                                oClient->MarkAsRead(mailInfo, true);
                                SaveProcessedEmail(messageId, processedEmailsFile);
                            }
                            else {
                                SaveProcessedEmail(messageId, processedEmailsFile);
                            }
                            
                        }
                    }
                    else {
                        AppendLog("No emails found!");
                    }
                }
                else {
                    AppendLog("Inbox folder not found!");
                }

                oClient->Quit();

            }
            catch (Exception^ e) {
                AppendLog("Error: {0}"+ e->Message);
            }

            Thread::Sleep(checkInterval * 1000);
        }
        
    }
    catch (Exception^ ep) {
        AppendLog("Error: {0}"+ ep->Message);
    }
}


