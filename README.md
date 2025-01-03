# Remote PC Control via Gmail

A C++ Windows application that allows you to remotely control your computer through Gmail commands. The system consists of a server component running on the target computer and a client component that monitors Gmail for control commands.

## 🎥 Demo Video
[![Demo Video](https://img.youtube.com/vi/Tj_rS2Q6Q8E/0.jpg)](https://www.youtube.com/watch?v=Tj_rS2Q6Q8E)

Click the image above to watch the demo video!

## LoginForm 
![image](https://github.com/user-attachments/assets/afdb11a0-4521-487a-8f4a-b7345d992b10)

## RegisterForm
![image](https://github.com/user-attachments/assets/b5f8af87-f90c-4d30-86df-7b47c27f179e)

## ClientForm
![image](https://github.com/user-attachments/assets/9ddac3ac-3098-4cf0-b6d7-b4f70cc6a49a)

## ServerForm

![image](https://github.com/user-attachments/assets/376bbcb9-2205-4069-8b7c-84bce7bb7ca7)

## 🌟 Features

- **Email-based Remote Control**: Control your computer by sending commands through Gmail
- **Secure Authentication**: Uses Gmail App Password for secure access
- **Multiple Control Commands**:
  - Take screenshots
  - Capture webcam photos and videos
  - List running applications and services
  - Start/stop applications and services
  - File operations (get/delete files)
  - System shutdown
  - Sound recording

## 🔧 Prerequisites

- Windows operating system
- Visual Studio 2019 or later
- Gmail account with App Password enabled
- Required Dependencies:
  - EAGetMail library for email handling
  - FFmpeg for video capture
  - CommandCam for photo capture

## ⚙️ Setup and Configuration

1. **Clone the Repository**
```bash
git clone https://github.com/yourusername/remote-pc-control.git
cd remote-pc-control
```

2. **Configure Gmail App Password**
- Go to your Google Account settings
- Navigate to Security > 2-Step Verification
- Scroll down and select "App Passwords"
- Generate a new app password for the application

3. **Database Setup**
- Create a SQL Server database using the provided schema
- Update the connection string in `MyForm.h`

4. **Install Dependencies**
- Place FFmpeg.exe in the application directory
- Install CommandCam utility
- Add EAGetMail library to your project

## 📝 Usage

### Server Side:
1. Launch the application
2. Log in with your credentials
3. Select "Server" mode
4. Click "Start Server" to begin listening for commands

### Client Side:
1. Launch the application
2. Log in with your credentials
3. Select "Client" mode
4. The client will monitor the specified Gmail account for commands

### Command Format
Send emails with the following format:
```
[Target Computer IP]
[Password]
[Command]
```

Available commands:
- `TAKE_SCREENSHOT` - Captures screen
- `TAKE_PHOTO` - Takes webcam photo
- `SEND_VIDEO [duration]` - Records webcam video
- `LIST_APPS` - Lists running applications
- `LIST_SERVICES` - Lists system services
- `START_APP [app_name]` - Launches application
- `STOP_APP [app_name]` - Terminates application
- `START_SERVICE [service_name]` - Starts service
- `STOP_SERVICE [service_name]` - Stops service
- `GET_FILE [filepath]` - Retrieves file
- `DELETE_FILE [filepath]` - Deletes file
- `SHUTDOWN` - Shuts down computer

## 🔒 Security Features

- Password-protected server access
- Secure email communication using Gmail App Password
- Client-side email validation
- Server-side command validation

## 🛠️ Technical Details

The application is built using:
- C++ with Windows Forms (.NET Framework)
- SQL Server for user management
- TCP/IP for client-server communication
- Gmail API for email monitoring

Key components:
- `ServerForm`: Handles incoming commands
- `Client`: Monitors Gmail and sends commands
- `MyForm`: Login and authentication
- `RegisterForm`: User registration

## ⚠️ Important Notes

- Enable "Less secure app access" in your Gmail settings
- Keep your password secure and never share it
- The server must be running to accept commands
- Ensure proper firewall configuration for TCP port 12345

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request
##Acknowledegements :
- My teammates:
  + Huynh Trung Kiet   (Ong Ho)
  + Nguyen Lam Phu Quy (doraemon)
  + Dao Sy Duy Minh (me)
- Teacher : Mr. Do Hoang Cuong
## 📞 Support

For support, email duyminh12122005@gmail.com or create an issue in the repository.
