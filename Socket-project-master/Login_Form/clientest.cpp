//#include "client.h"
//
//using namespace System;
//using namespace System::Globalization;
//using namespace System::IO;
//using namespace System::Threading;
//using namespace System::Net;
//using namespace System::Net::Mail;
//using namespace System::Net::Sockets;
//using namespace System::Text;
//using namespace EAGetMail; // Thư viện EAGetMail cho email
//
//// Hàm tạo tên file tạm thời
//void LoginForm::client::StartApplication(String^ emailAddress, String^ appPassword, String^ specificSender, int checkInterval) {
//    RunClient(emailAddress, appPassword, specificSender, checkInterval);
//}
//
//// Hàm tạo tên file tạm thời
//String^ LoginForm::client::GenerateFileName(int sequence) {
//    DateTime currentDateTime = DateTime::Now;
//    return String::Format("{0}-{1:000}-{2:000}.txt",
//        currentDateTime.ToString("yyyyMMddHHmmss", gcnew CultureInfo("en-US")),
//        currentDateTime.Millisecond,
//        sequence);
//}
//
//// Hàm tìm thư mục Inbox
//Imap4Folder^ LoginForm::client::FindInboxFolder(array<Imap4Folder^>^ folders) {
//    for each (Imap4Folder ^ folder in folders) {
//        if (folder->Name->Equals("INBOX", StringComparison::OrdinalIgnoreCase)) {
//            return folder;
//        }
//    }
//    return nullptr;
//}
//
//// Hàm kiểm tra xem email đã được xử lý chưa
//bool LoginForm::client::IsEmailProcessed(String^ messageId, String^ processedEmailsFile) {
//    if (!File::Exists(processedEmailsFile)) {
//        return false; // Nếu file chưa tồn tại, email chưa được xử lý
//    }
//
//    array<String^>^ processedEmails = File::ReadAllLines(processedEmailsFile);
//    for each (String ^ id in processedEmails) {
//        if (id->Equals(messageId)) {
//            return true; // Email đã được xử lý
//        }
//    }
//    return false;
//}
//
//// Hàm lưu lại email đã xử lý
//void LoginForm::client::SaveProcessedEmail(String^ messageId, String^ processedEmailsFile) {
//    File::AppendAllText(processedEmailsFile, messageId + Environment::NewLine);
//}
//
//// Kiểm tra xem email có phải từ người gửi cụ thể không
//bool LoginForm::client::IsEmailFromSpecificSender(EAGetMail::Mail^ oMail, String^ specificSender) {
//    return oMail->From->Address->Equals(specificSender, StringComparison::OrdinalIgnoreCase);
//}
//
//// Nhận hình ảnh từ stream
//void LoginForm::client::ReceiveImage(NetworkStream^ stream) {
//    array<Byte>^ sizeBuffer = gcnew array<Byte>(4);
//    stream->Read(sizeBuffer, 0, sizeBuffer->Length);
//    int imageSize = BitConverter::ToInt32(sizeBuffer, 0);
//    array<Byte>^ imageData = gcnew array<Byte>(imageSize);
//    stream->Read(imageData, 0, imageData->Length);
//    String^ imagePath = "screenshot_" + DateTime::Now.ToString("yyyyMMdd_HHmmss") + ".png";
//    File::WriteAllBytes(imagePath, imageData);
//    Console::WriteLine("Image received and saved as: " + imagePath);
//}
//
//// Hàm gửi email
//void LoginForm::client::SendEmail(String^ recipient, String^ subject, String^ body, String^ attachmentPath) {
//    try {
//        if (String::IsNullOrEmpty(recipient) || !System::Text::RegularExpressions::Regex::IsMatch(recipient, "^\\S+@\\S+\\.\\S+$")) {
//            throw gcnew ArgumentException("Địa chỉ email không hợp lệ: " + recipient);
//        }
//
//        if (String::IsNullOrEmpty(subject)) {
//            subject = "No Subject";
//        }
//        if (String::IsNullOrEmpty(body)) {
//            body = "No Content";
//        }
//
//        SmtpClient^ smtpClient = gcnew SmtpClient("smtp.gmail.com", 587);
//        smtpClient->Credentials = gcnew NetworkCredential("huynhtrungkiet09032005@gmail.com", "ifztigzwrspjwhch");
//        smtpClient->EnableSsl = true;
//
//        MailMessage^ mailMessage = gcnew MailMessage();
//        mailMessage->From = gcnew System::Net::Mail::MailAddress("huynhtrungkiet09032005@gmail.com");
//        mailMessage->To->Add(recipient);
//        mailMessage->Subject = subject;
//        mailMessage->Body = body;
//
//        if (!String::IsNullOrEmpty(attachmentPath)) {
//            if (System::IO::File::Exists(attachmentPath)) {
//                mailMessage->Attachments->Add(gcnew System::Net::Mail::Attachment(attachmentPath));
//            }
//            else {
//                String^ tempFilePath = "response.txt";
//                StreamWriter^ writer = gcnew StreamWriter(tempFilePath);
//                writer->Write(attachmentPath);
//                writer->Close();
//                mailMessage->Attachments->Add(gcnew System::Net::Mail::Attachment(tempFilePath));
//            }
//        }
//
//        smtpClient->Send(mailMessage);
//        Console::WriteLine("Email sent successfully to " + recipient);
//    }
//    catch (Exception^ ex) {
//        Console::WriteLine("Error sending email: " + ex->Message);
//        Console::WriteLine("Stack Trace: " + ex->StackTrace);
//    }
//}
//
//// Trích xuất địa chỉ IP từ nội dung email
//String^ LoginForm::client::ExtractIpFromEmailBody(String^ emailBody) {
//    int ipStartIndex = emailBody->IndexOf("IP: ");
//    if (ipStartIndex == -1) {
//        return nullptr; // Không tìm thấy địa chỉ IP
//    }
//
//    int ipEndIndex = emailBody->IndexOf(Environment::NewLine, ipStartIndex);
//    if (ipEndIndex == -1) {
//        ipEndIndex = emailBody->Length;
//    }
//
//    return emailBody->Substring(ipStartIndex + 4, ipEndIndex - ipStartIndex - 4);
//}
//
//// Trích xuất lệnh từ nội dung email
//String^ LoginForm::client::ExtractCommandFromEmailBody(String^ emailBody) {
//    if (String::IsNullOrEmpty(emailBody)) {
//        return nullptr; // Không có lệnh
//    }
//
//    int firstNewLineIndex = emailBody->IndexOf(Environment::NewLine);
//    if (firstNewLineIndex == -1) {
//        return nullptr; // Không có lệnh
//    }
//
//    String^ remainingBody = emailBody->Substring(firstNewLineIndex + Environment::NewLine->Length)->Trim();
//    return remainingBody;
//}
//
//// Xử lý phản hồi từ server
//void LoginForm::client::handleResponse(String^ command, NetworkStream^ stream, String^ recipientEmail) {
//    if (command->Equals("TAKE_SCREENSHOT", StringComparison::OrdinalIgnoreCase) ||
//        command->Equals("TAKE_PHOTO", StringComparison::OrdinalIgnoreCase) ||
//        command->Equals("SEND_VIDEO", StringComparison::OrdinalIgnoreCase)) {
//
//        array<Byte>^ sizeBuffer = gcnew array<Byte>(4);
//        int bytesRead = stream->Read(sizeBuffer, 0, sizeBuffer->Length);
//        if (bytesRead != sizeBuffer->Length) {
//            Console::WriteLine("Failed to receive image/video size.");
//            return;
//        }
//
//        int dataSize = BitConverter::ToInt32(sizeBuffer, 0);
//        if (dataSize <= 0) {
//            Console::WriteLine("Invalid data size received.");
//            return;
//        }
//
//        array<Byte>^ data = gcnew array<Byte>(dataSize);
//        int totalBytesRead = 0;
//        while (totalBytesRead < dataSize) {
//            int read = stream->Read(data, totalBytesRead, dataSize - totalBytesRead);
//            if (read <= 0) {
//                Console::WriteLine("Failed to receive complete data.");
//                return;
//            }
//            totalBytesRead += read;
//        }
//
//        String^ filePath = (command->Equals("SEND_VIDEO") ? "Video_" : "Image_") + DateTime::Now.ToString("yyyyMMdd_HHmmss") +
//            (command->Equals("SEND_VIDEO") ? ".mp4" : ".png");
//        File::WriteAllBytes(filePath, data);
//        Console::WriteLine("Data received and saved as: " + filePath);
//
//        String^ subject = (command->Equals("SEND_VIDEO") ? "Video" : "Image") + " from Server";
//        String^ body = "Here is the " + (command->Equals("SEND_VIDEO") ? "video" : "image") + " you requested.";
//        SendEmail(recipientEmail, subject, body, filePath);
//    }
//    else if (command->Contains("GET_FILE")) {
//        Console::WriteLine("Requesting file from server...");
//
//        array<Byte>^ sizeBuffer = gcnew array<Byte>(4);
//        int bytesRead = stream->Read(sizeBuffer, 0, sizeBuffer->Length);
//        if (bytesRead != sizeBuffer->Length) {
//            Console::WriteLine("Failed to receive file size.");
//            return;
//        }
//
//        int fileSize = BitConverter::ToInt32(sizeBuffer, 0);
//        if (fileSize <= 0) {
//            Console::WriteLine("Invalid file size received.");
//            return;
//        }
//
//        array<Byte>^ fileData = gcnew array<Byte>(fileSize);
//        int totalBytesRead = 0;
//        while (totalBytesRead < fileSize) {
//            int read = stream->Read(fileData, totalBytesRead, fileSize - totalBytesRead);
//            if (read <= 0) {
//                Console::WriteLine("Failed to receive complete file data.");
//                return;
//            }
//            totalBytesRead += read;
//        }
//
//        String^ filePath = "received_file_" + DateTime::Now.ToString("yyyyMMdd_HHmmss") + ".dat";
//        File::WriteAllBytes(filePath, fileData);
//        Console::WriteLine("File received and saved as: " + filePath);
//
//        String^ subject = "File Received from Server";
//        String^ body = "Here is the file you requested.";
//        SendEmail(recipientEmail, subject, body, filePath);
//    }
//    else {
//        array<Byte>^ buffer = gcnew array<Byte>(65536);
//        int bytesRead = stream->Read(buffer, 0, buffer->Length);
//        if (bytesRead > 0) {
//            String^ response = Encoding::UTF8->GetString(buffer, 0, bytesRead);
//            Console::WriteLine("Response from server:\r\n{0}", response);
//
//            String^ filePath = "response_" + DateTime::Now.ToString("yyyyMMdd_HHmmss") + ".txt";
//            File::WriteAllText(filePath, response);
//            String^ subject = "Response from Server";
//            String^ body = "Here is the response from the server saved in a text file.";
//            SendEmail(recipientEmail, subject, body, filePath);
//            Console::WriteLine("Response saved to file and sent via email.");
//        }
//        else {
//            Console::WriteLine("No response from server.");
//        }
//    }
//}
//
//// Hàm chạy client với các tham số truyền vào
//void LoginForm::client::RunClient(String^ emailAddress, String^ appPassword, String^ specificSender, int checkInterval) {
//    try {
//        DateTime startTime = DateTime::Now;
//        String^ processedEmailsFile = Path::Combine(Directory::GetCurrentDirectory(), "processed_emails.txt");
//
//        MailServer^ oServer = gcnew MailServer("imap.gmail.com", emailAddress, appPassword, ServerProtocol::Imap4);
//        oServer->SSLConnection = true;
//        oServer->Port = 993;
//
//        Console::WriteLine("Connecting to IMAP server...");
//
//        MailClient^ oClient = gcnew MailClient("TryIt");
//
//        while (true) {
//            try {
//                oClient->Connect(oServer);
//                Console::WriteLine("Connected to IMAP server");
//                array<Imap4Folder^>^ folders = oClient->GetFolders();
//                Imap4Folder^ inboxFolder = FindInboxFolder(folders);
//
//                if (inboxFolder != nullptr) {
//                    oClient->SelectFolder(inboxFolder);
//                    array<MailInfo^>^ infos = oClient->GetMailInfos();
//                    Console::WriteLine("Total {0} email(s)\r\n", infos->Length);
//
//                    if (infos->Length > 0) {
//                        for (int i = infos->Length - 1; i >= 0; i--) {
//                            MailInfo^ mailInfo = infos[i];
//                            String^ messageId = mailInfo->UIDL;
//
//                            if (IsEmailProcessed(messageId, processedEmailsFile)) {
//                                Console::WriteLine("Email already processed. Skipping...");
//                                continue;
//                            }
//
//                            EAGetMail::Mail^ oMail = oClient->GetMail(mailInfo);
//                            DateTime sentTime = oMail->ReceivedDate;
//
//                            if (sentTime < startTime) {
//                                Console::WriteLine("Found an email sent before application started. Stopping...");
//                                break;
//                            }
//
//                            Console::WriteLine("Processing email sent at: {0}", sentTime);
//
//                            if (specificSender->Length == 0 || IsEmailFromSpecificSender(oMail, specificSender)) {
//                                Console::WriteLine("From: {0}", oMail->From->ToString());
//                                Console::WriteLine("Subject: {0}\r\n", oMail->Subject);
//                                Console::WriteLine("Body: {0}\r\n", oMail->TextBody);
//
//                                String^ ip_add = ExtractIpFromEmailBody(oMail->TextBody->Trim());
//                                String^ command = ExtractCommandFromEmailBody(oMail->TextBody->Trim());
//
//                                try {
//                                    TcpClient^ client = gcnew TcpClient();
//                                    client->Connect(ip_add, 12345);
//                                    NetworkStream^ stream = client->GetStream();
//
//                                    array<Byte>^ data = Encoding::UTF8->GetBytes(command);
//                                    stream->Write(data, 0, data->Length);
//                                    stream->Flush();
//
//                                    handleResponse(command, stream, specificSender);
//                                    stream->Close();
//                                    client->Close();
//                                }
//                                catch (Exception^ ex) {
//                                    Console::WriteLine("Error connecting to server at IP {0}: {1}", ip_add, ex->Message);
//                                    String^ subject = "Error: Failed to Connect to Server";
//                                    String^ body = "The IP address provided in your email (" + ip_add +
//                                        ") could not be connected to. Please verify the IP address and try again.\r\n\r\nError Details:\r\n" + ex->Message;
//                                    SendEmail(oMail->From->Address, subject, body, nullptr);
//                                }
//
//                                oClient->MarkAsRead(mailInfo, true);
//                                SaveProcessedEmail(messageId, processedEmailsFile);
//                            }
//                            else {
//                                Console::WriteLine("Email is not from the specified sender: {0}\r\n", oMail->From->ToString());
//                            }
//                        }
//                    }
//                    else {
//                        Console::WriteLine("No emails found!");
//                    }
//                }
//                else {
//                    Console::WriteLine("Inbox folder not found!");
//                }
//
//                oClient->Quit();
//                Console::WriteLine("Waiting for the next check...");
//            }
//            catch (Exception^ e) {
//                Console::WriteLine("Error: {0}", e->Message);
//            }
//
//            Thread::Sleep(checkInterval * 1000);
//        }
//    }
//    catch (Exception^ ep) {
//        Console::WriteLine("Error: {0}", ep->Message);
//    }
//}
//
//// Hàm chính để gọi RunClient từ nơi khác
//int main(array<System::String^>^ args) {
//    String^ emailAddress = "huynhtrungkiet09032005@gmail.com";
//    String^ appPassword = "ifztigzwrspjwhch";
//    String^ specificSender = ""; // Người gửi cụ thể (có thể thay đổi)
//    int checkInterval = 60; // Khoảng thời gian kiểm tra (mặc định là 60 giây)
//
//    LoginForm::client^ myClient = gcnew LoginForm::client(emailAddress, appPassword, specificSender, checkInterval);
//    Application::Run(myClient);
//    return 0;
//}
