

#include "MyForm.h"
#include "Client.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    //String^ emailAddress = "huynhtrungkiet09032005@gmail.com";
    //String^ appPassword = "ifztigzwrspjwhch";
    //String^ specificSender = ""; // Người gửi cụ thể (có thể thay đổi)
    //int checkInterval = 0; // Khoảng thời gian kiểm tra (mặc định là 60 giây)

    //// Instantiate the Client class similar to MyForm
    //LoginForm::Client myClient(emailAddress, appPassword, specificSender, checkInterval); // Create an instance of Client
    //Application::Run(% myClient); // Run the Client instance

    // Sử dụng đúng tên namespace và lớp MyForm
    LoginForm::MyForm form;
    Application::Run(% form);
    //StartServer(); // Gọi hàm khởi động server khi cần thiết
    return 0;


}