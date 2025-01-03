#pragma once

namespace LoginForm {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Text::RegularExpressions;
    using namespace System::Data::SqlClient;

    public ref class RegisterForm1 : public System::Windows::Forms::Form {
    public:
        RegisterForm1(void) {
            InitializeComponent();
            this->StartPosition = FormStartPosition::CenterScreen;

            // Initialize the timer
            this->passwordStrengthTimer = gcnew System::Windows::Forms::Timer();
            this->passwordStrengthTimer->Interval = 15; // Set the interval for the timer
            this->passwordStrengthTimer->Tick += gcnew System::EventHandler(this, &RegisterForm1::OnPasswordStrengthTimerTick);

            this->isTimerRunning = false; // Initialize the timer running flag
        }



    protected:
        ~RegisterForm1() {
            if (components) {
                delete components;
            }
        }

    private: System::Windows::Forms::Timer^ passwordStrengthTimer;
    private: int targetStrength; // Target strength for the progress bar
    private: bool isTimerRunning; // Track if the timer is running
           // Thêm một Timer để tạo hiệu ứng fade-in
    private: System::Windows::Forms::Timer^ fadeInTimer;
    private: int opacityStep = 5; // Giá trị thay đổi độ mờ

    private:
        System::Windows::Forms::PictureBox^ logoBox;
        System::Windows::Forms::ToolTip^ toolTip;
        System::Windows::Forms::Panel^ panel1;
        System::Windows::Forms::Button^ cancelButton;
        System::Windows::Forms::Button^ registerButton;
        System::Windows::Forms::Label^ passwordStrengthLabel;
        System::Windows::Forms::ProgressBar^ passwordStrengthBar; // Standard ProgressBar
        System::Windows::Forms::CheckBox^ showPasswordCheckBox;
        System::Windows::Forms::TextBox^ userPasswordTextBox;
        System::Windows::Forms::TextBox^ appKeyTextBox;
        System::Windows::Forms::TextBox^ receiverEmailTextBox;
        System::Windows::Forms::TextBox^ senderEmailTextBox;
        System::Windows::Forms::Panel^ mainPanel;
        System::ComponentModel::IContainer^ components;

        void InitializeComponent(void) {
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(RegisterForm1::typeid));
            this->toolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
            this->panel1 = (gcnew System::Windows::Forms::Panel());
            this->cancelButton = (gcnew System::Windows::Forms::Button());
            this->registerButton = (gcnew System::Windows::Forms::Button());
            this->passwordStrengthLabel = (gcnew System::Windows::Forms::Label());
            this->passwordStrengthBar = (gcnew System::Windows::Forms::ProgressBar());
            this->showPasswordCheckBox = (gcnew System::Windows::Forms::CheckBox());
            this->userPasswordTextBox = (gcnew System::Windows::Forms::TextBox());
            this->appKeyTextBox = (gcnew System::Windows::Forms::TextBox());
            this->receiverEmailTextBox = (gcnew System::Windows::Forms::TextBox());
            this->senderEmailTextBox = (gcnew System::Windows::Forms::TextBox());
            this->mainPanel = (gcnew System::Windows::Forms::Panel());
            this->panel1->SuspendLayout();
            this->mainPanel->SuspendLayout();
            this->SuspendLayout();
            // 
            // panel1
            // 
            this->panel1->AutoScroll = true;
            this->panel1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"panel1.BackgroundImage")));
            this->panel1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->panel1->Controls->Add(this->cancelButton);
            this->panel1->Location = System::Drawing::Point(8, -1);
            this->panel1->Name = L"panel1";
            this->panel1->Size = System::Drawing::Size(308, 621);
            this->panel1->TabIndex = 1;
            this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &RegisterForm1::panel1_Paint_2);
            // 
            // cancelButton
            // 
            this->cancelButton->BackColor = System::Drawing::Color::White;
            this->cancelButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->cancelButton->ForeColor = System::Drawing::Color::Salmon;
            this->cancelButton->Location = System::Drawing::Point(26, 340);
            this->cancelButton->Name = L"cancelButton";
            this->cancelButton->Size = System::Drawing::Size(255, 40);
            this->cancelButton->TabIndex = 13;
            this->cancelButton->Text = L"Back to Login";
            this->cancelButton->UseVisualStyleBackColor = false;
            this->cancelButton->Click += gcnew System::EventHandler(this, &RegisterForm1::cancelButton_Click);
            // 
            // registerButton
            // 
            this->registerButton->BackColor = System::Drawing::Color::Coral;
            this->registerButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->registerButton->ForeColor = System::Drawing::Color::White;
            this->registerButton->Location = System::Drawing::Point(161, 511);
            this->registerButton->Name = L"registerButton";
            this->registerButton->Size = System::Drawing::Size(255, 40);
            this->registerButton->TabIndex = 12;
            this->registerButton->Text = L"Create Account";
            this->registerButton->UseVisualStyleBackColor = false;
            this->registerButton->Click += gcnew System::EventHandler(this, &RegisterForm1::registerButton_Click);
            // 
            // passwordStrengthLabel
            // 
            this->passwordStrengthLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8));
            this->passwordStrengthLabel->Location = System::Drawing::Point(83, 489);
            this->passwordStrengthLabel->Name = L"passwordStrengthLabel";
            this->passwordStrengthLabel->Size = System::Drawing::Size(360, 20);
            this->passwordStrengthLabel->TabIndex = 11;
            this->passwordStrengthLabel->Text = L"Password Strength: Weak";
            // 
            // passwordStrengthBar
            // 
            this->passwordStrengthBar->Location = System::Drawing::Point(86, 481);
            this->passwordStrengthBar->Name = L"passwordStrengthBar";
            this->passwordStrengthBar->Size = System::Drawing::Size(414, 5);
            this->passwordStrengthBar->TabIndex = 0;
            // 
            // showPasswordCheckBox
            // 
            this->showPasswordCheckBox->AutoSize = true;
            this->showPasswordCheckBox->Location = System::Drawing::Point(86, 463);
            this->showPasswordCheckBox->Name = L"showPasswordCheckBox";
            this->showPasswordCheckBox->Size = System::Drawing::Size(102, 17);
            this->showPasswordCheckBox->TabIndex = 9;
            this->showPasswordCheckBox->Text = L"Show Password";
            this->showPasswordCheckBox->CheckedChanged += gcnew System::EventHandler(this, &RegisterForm1::showPasswordCheckBox_CheckedChanged);
            // 
            // userPasswordTextBox
            // 
            this->userPasswordTextBox->BackColor = System::Drawing::Color::LightSalmon;
            this->userPasswordTextBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->userPasswordTextBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->userPasswordTextBox->Location = System::Drawing::Point(86, 433);
            this->userPasswordTextBox->Name = L"userPasswordTextBox";
            this->userPasswordTextBox->PasswordChar = '*';
            this->userPasswordTextBox->Size = System::Drawing::Size(414, 25);
            this->userPasswordTextBox->TabIndex = 8;
            this->userPasswordTextBox->TextChanged += gcnew System::EventHandler(this, &RegisterForm1::UpdatePasswordStrength);
            // 
            // appKeyTextBox
            // 
            this->appKeyTextBox->BackColor = System::Drawing::Color::LightSalmon;
            this->appKeyTextBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->appKeyTextBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->appKeyTextBox->Location = System::Drawing::Point(85, 358);
            this->appKeyTextBox->Name = L"appKeyTextBox";
            this->appKeyTextBox->Size = System::Drawing::Size(414, 25);
            this->appKeyTextBox->TabIndex = 6;
            // 
            // receiverEmailTextBox
            // 
            this->receiverEmailTextBox->BackColor = System::Drawing::Color::LightSalmon;
            this->receiverEmailTextBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->receiverEmailTextBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->receiverEmailTextBox->Location = System::Drawing::Point(86, 277);
            this->receiverEmailTextBox->Name = L"receiverEmailTextBox";
            this->receiverEmailTextBox->Size = System::Drawing::Size(414, 25);
            this->receiverEmailTextBox->TabIndex = 4;
            this->receiverEmailTextBox->TextChanged += gcnew System::EventHandler(this, &RegisterForm1::receiverEmailTextBox_TextChanged);
            // 
            // senderEmailTextBox
            // 
            this->senderEmailTextBox->BackColor = System::Drawing::Color::LightSalmon;
            this->senderEmailTextBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->senderEmailTextBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->senderEmailTextBox->Location = System::Drawing::Point(85, 197);
            this->senderEmailTextBox->Name = L"senderEmailTextBox";
            this->senderEmailTextBox->Size = System::Drawing::Size(414, 25);
            this->senderEmailTextBox->TabIndex = 2;
            this->senderEmailTextBox->TextChanged += gcnew System::EventHandler(this, &RegisterForm1::senderEmailTextBox_TextChanged);
            // 
            // mainPanel
            // 
            this->mainPanel->BackColor = System::Drawing::Color::Transparent;
            this->mainPanel->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"mainPanel.BackgroundImage")));
            this->mainPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
            this->mainPanel->Controls->Add(this->senderEmailTextBox);
            this->mainPanel->Controls->Add(this->receiverEmailTextBox);
            this->mainPanel->Controls->Add(this->appKeyTextBox);
            this->mainPanel->Controls->Add(this->userPasswordTextBox);
            this->mainPanel->Controls->Add(this->showPasswordCheckBox);
            this->mainPanel->Controls->Add(this->passwordStrengthBar);
            this->mainPanel->Controls->Add(this->passwordStrengthLabel);
            this->mainPanel->Controls->Add(this->registerButton);
            this->mainPanel->Location = System::Drawing::Point(314, -4);
            this->mainPanel->Name = L"mainPanel";
            this->mainPanel->Size = System::Drawing::Size(587, 616);
            this->mainPanel->TabIndex = 0;
            // 
            // RegisterForm1
            // 
            this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(245)),
                static_cast<System::Int32>(static_cast<System::Byte>(245)));
            this->ClientSize = System::Drawing::Size(902, 605);
            this->Controls->Add(this->panel1);
            this->Controls->Add(this->mainPanel);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->Name = L"RegisterForm1";
            this->Text = L"Registration";
            this->Load += gcnew System::EventHandler(this, &RegisterForm1::RegisterForm1_Load);
            this->panel1->ResumeLayout(false);
            this->mainPanel->ResumeLayout(false);
            this->mainPanel->PerformLayout();
            this->ResumeLayout(false);

        }

        void showPasswordCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
            this->userPasswordTextBox->PasswordChar = this->showPasswordCheckBox->Checked ? '\0' : '*';
        }

        void UpdatePasswordStrength(System::Object^ sender, System::EventArgs^ e) {
            String^ password = this->userPasswordTextBox->Text;
            int strength = 0;

            if (password->Length >= 8) strength += 25;
            if (Regex::IsMatch(password, "[A-Z]")) strength += 25;
            if (Regex::IsMatch(password, "[a-z]")) strength += 25;
            if (Regex::IsMatch(password, "[0-9]")) strength += 25;

            this->targetStrength = strength;

            if (!isTimerRunning) {
                this->passwordStrengthTimer->Start();
                isTimerRunning = true;
            }

            String^ strengthText = L"Password Strength: ";
            if (strength < 50) strengthText += L"Weak";
            else if (strength < 75) strengthText += L"Medium";
            else strengthText += L"Strong";

            this->passwordStrengthLabel->Text = strengthText;
        }

        void OnPasswordStrengthTimerTick(System::Object^ sender, System::EventArgs^ e) {
            int step = 1;

            if (this->passwordStrengthBar->Value < this->targetStrength) {
                this->passwordStrengthBar->Value = Math::Min(this->passwordStrengthBar->Value + step, this->targetStrength);
            }
            else if (this->passwordStrengthBar->Value > this->targetStrength) {
                this->passwordStrengthBar->Value = Math::Max(this->passwordStrengthBar->Value - step, this->targetStrength);
            }

            // Stop the timer if we reached the target strength
            if (this->passwordStrengthBar->Value == this->targetStrength) {
                this->passwordStrengthTimer->Stop();
                isTimerRunning = false;
            }
        }


        bool ValidateEmail(String^ email) {
            return Regex::IsMatch(email, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        }
        void ValidateInputFields() {
            // Validate sender email
            if (!ValidateEmail(this->senderEmailTextBox->Text)) {
                this->senderEmailTextBox->BackColor = System::Drawing::Color::LightCoral; // Invalid input
            }
            else {
                this->senderEmailTextBox->BackColor = System::Drawing::Color::LightGreen; // Valid input
            }

            // Validate receiver email
            if (!ValidateEmail(this->receiverEmailTextBox->Text)) {
                this->receiverEmailTextBox->BackColor = System::Drawing::Color::LightCoral;
            }
            else {
                this->receiverEmailTextBox->BackColor = System::Drawing::Color::LightGreen;
            }

            // Validate password length
            if (this->userPasswordTextBox->Text->Length < 8) {
                this->userPasswordTextBox->BackColor = System::Drawing::Color::LightCoral;
            }
            else {
                this->userPasswordTextBox->BackColor = System::Drawing::Color::LightGreen;
            }
            if (this->appKeyTextBox->Text->Length == 0) {
                this->appKeyTextBox->BackColor = System::Drawing::Color::LightCoral;
            }
            else {
                this->appKeyTextBox->BackColor = System::Drawing::Color::LightGreen;
            }
        }


        void registerButton_Click(System::Object^ sender, System::EventArgs^ e) {
            ValidateInputFields(); // Validate inputs before proceeding
            // Validate user inputs
            if (!ValidateEmail(this->senderEmailTextBox->Text)) {
                MessageBox::Show("Please enter a valid sender email address.", "Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            if (!ValidateEmail(this->receiverEmailTextBox->Text)) {
                MessageBox::Show("Please enter a valid receiver email address.", "Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            if (this->userPasswordTextBox->Text->Length < 8) {
                MessageBox::Show("Password must be at least 8 characters long.", "Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            // Database insertion logic
            try {
                String^ connString = "Data Source=sqldatabasedm.database.windows.net;Initial Catalog=SampleDB;User ID=sqladmin;Password=sql123@gmail.com;Connect Timeout=60;Encrypt=True";
                SqlConnection^ sqlConn = gcnew SqlConnection(connString);
                sqlConn->Open();

                String^ query = "INSERT INTO Users (Sender_Email, Receiver_Email, Application_Key, Password) VALUES (@SenderEmail, @ReceiverEmail, @ApplicationKey, @Password)";
                SqlCommand^ cmd = gcnew SqlCommand(query, sqlConn);
                cmd->Parameters->AddWithValue("@SenderEmail", this->senderEmailTextBox->Text);
                cmd->Parameters->AddWithValue("@ReceiverEmail", this->receiverEmailTextBox->Text);
                cmd->Parameters->AddWithValue("@ApplicationKey", this->appKeyTextBox->Text);
                cmd->Parameters->AddWithValue("@Password", this->userPasswordTextBox->Text);

                int rowsAffected = cmd->ExecuteNonQuery();

                if (rowsAffected > 0) {
                    MessageBox::Show("Registration successful!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
                    this->Close(); // Close the registration form after successful registration
                }
                else {
                    MessageBox::Show("Registration failed. Please try again.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                }

                sqlConn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Database error: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
        // Confirm before closing the form
        void cancelButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (MessageBox::Show("Are you sure you want to discard your changes?", "Confirm", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes) {
                this->Close();
            }
        }
        void TextBox_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
            TextBox^ textBox = dynamic_cast<TextBox^>(sender);
            if (textBox) {
                textBox->BackColor = System::Drawing::Color::LightYellow;
            }
        }

        void TextBox_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
            TextBox^ textBox = dynamic_cast<TextBox^>(sender);
            if (textBox) {
                textBox->BackColor = System::Drawing::Color::LightSalmon; // original color
            }
        }

        // Button Mouse Enter and Leave Handlers for Register Button
        void RegisterButton_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
            registerButton->BackColor = System::Drawing::Color::LightCoral; // Change color on hover
            registerButton->ForeColor = System::Drawing::Color::White; // Change text color on hover
        }

        void RegisterButton_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
            registerButton->BackColor = System::Drawing::Color::Coral; // Original color
            registerButton->ForeColor = System::Drawing::Color::White; // Original text color
        }

        // Button Mouse Enter and Leave Handlers for Cancel Button
        void CancelButton_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
            cancelButton->BackColor = System::Drawing::Color::LightSalmon; // Change color on hover
            cancelButton->ForeColor = System::Drawing::Color::White; // Change text color on hover
        }

        void CancelButton_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
            cancelButton->BackColor = System::Drawing::Color::White; // Original color
            cancelButton->ForeColor = System::Drawing::Color::Salmon; // Original text color
        }

        // Button Mouse Down and Up Handlers (can be shared)
        void Button_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            Button^ button = dynamic_cast<Button^>(sender);
            if (button) {
                button->BackColor = System::Drawing::Color::DarkSalmon;
                button->ForeColor = System::Drawing::Color::White; // Change text color on click
                button->Size = System::Drawing::Size(button->Width - 5, button->Height - 5); // Reduce size
            }
        }

        void Button_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            Button^ button = dynamic_cast<Button^>(sender);
            if (button) {
                // Restore original color based on which button was clicked
                if (button == registerButton) {
                    button->BackColor = System::Drawing::Color::Coral; // Register button color
                    button->ForeColor = System::Drawing::Color::White; // Restore text color
                }
                else if (button == cancelButton) {
                    button->BackColor = System::Drawing::Color::White; // Cancel button color
                    button->ForeColor = System::Drawing::Color::Salmon; // Restore text color
                }
                button->Size = System::Drawing::Size(button->Width + 5, button->Height + 5); // Restore size
            }
        }

        void RegisterForm1_Load(System::Object^ sender, System::EventArgs^ e) {
            this->Opacity = 0; // Bắt đầu với độ mờ 0
            this->senderEmailTextBox->Focus();

            // Khởi tạo và cấu hình Timer
            fadeInTimer = gcnew System::Windows::Forms::Timer();
            fadeInTimer->Interval = 50; // Thay đổi độ mờ mỗi 50ms
            fadeInTimer->Tick += gcnew System::EventHandler(this, &RegisterForm1::FadeInTimer_Tick);
            fadeInTimer->Start(); // Bắt đầu Timer
        }

        // Hàm xử lý sự kiện Timer
        void FadeInTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
            if (this->Opacity < 1.0) {
                this->Opacity += 0.05; // Tăng độ mờ
            }
            else {
                fadeInTimer->Stop(); // Dừng Timer khi đạt độ mờ tối đa
            }
        }

    private: System::Void passwordStrengthBar_Click(System::Object^ sender, System::EventArgs^ e) {}
    private: System::Void headerLabel_Click(System::Object^ sender, System::EventArgs^ e) {}
    private: System::Void mainPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {}
    private: System::Void panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {}
    private: System::Void senderEmailTextBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
    }

private: System::Void panel1_Paint_1(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
}
private: System::Void headerLabel_Click_1(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void receiverEmailTextBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void panel1_Paint_2(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
}
};
}