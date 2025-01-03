#include "ServerForm.h"
#include "UserInfo.h"
#include "RegisterForm1.h" // Include the new registration form header
#include "SelectionForm.h"
#pragma once

namespace LoginForm {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Data::SqlClient;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::TextBox^ textBox1;
        System::Windows::Forms::CheckBox^ checkBox1;
        System::Windows::Forms::TextBox^ textBox2;
        System::Windows::Forms::Label^ label4;
        System::Windows::Forms::Label^ label5;
        System::Windows::Forms::Button^ button1;
        System::Windows::Forms::Button^ buttonShowPassword;
        System::Windows::Forms::Button^ buttonRegister; // New button for registration

        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
            this->textBox2 = (gcnew System::Windows::Forms::TextBox());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->buttonShowPassword = (gcnew System::Windows::Forms::Button());
            this->buttonRegister = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // textBox1
            // 
            this->textBox1->BackColor = System::Drawing::Color::DarkSalmon;
            this->textBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->textBox1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F));
            this->textBox1->Location = System::Drawing::Point(48, 326);
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(239, 19);
            this->textBox1->TabIndex = 3;
            this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
            this->textBox1->Enter += gcnew System::EventHandler(this, &MyForm::textBox1_Enter);
            this->textBox1->Leave += gcnew System::EventHandler(this, &MyForm::textBox1_Leave);
            // 
            // checkBox1
            // 
            this->checkBox1->AutoSize = true;
            this->checkBox1->BackColor = System::Drawing::Color::Transparent;
            this->checkBox1->Location = System::Drawing::Point(40, 418);
            this->checkBox1->Name = L"checkBox1";
            this->checkBox1->Size = System::Drawing::Size(71, 17);
            this->checkBox1->TabIndex = 4;
            this->checkBox1->Text = L"I agree to";
            this->checkBox1->UseVisualStyleBackColor = false;
            // 
            // textBox2
            // 
            this->textBox2->BackColor = System::Drawing::Color::DarkSalmon;
            this->textBox2->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->textBox2->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F));
            this->textBox2->Location = System::Drawing::Point(49, 388);
            this->textBox2->Name = L"textBox2";
            this->textBox2->PasswordChar = '*';
            this->textBox2->Size = System::Drawing::Size(239, 19);
            this->textBox2->TabIndex = 5;
            this->textBox2->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox2_TextChanged);
            this->textBox2->Enter += gcnew System::EventHandler(this, &MyForm::textBox2_Enter);
            this->textBox2->Leave += gcnew System::EventHandler(this, &MyForm::textBox2_Leave);
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F));
            this->label4->Location = System::Drawing::Point(193, 185);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(0, 19);
            this->label4->TabIndex = 6;
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->BackColor = System::Drawing::Color::Transparent;
            this->label5->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F));
            this->label5->ForeColor = System::Drawing::Color::Orange;
            this->label5->Location = System::Drawing::Point(107, 416);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(110, 19);
            this->label5->TabIndex = 7;
            this->label5->Text = L"Terms and Policy";
            this->label5->Click += gcnew System::EventHandler(this, &MyForm::label5_Click);
            // 
            // button1
            // 
            this->button1->BackColor = System::Drawing::Color::LightSalmon;
            this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->button1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F));
            this->button1->ForeColor = System::Drawing::SystemColors::ButtonFace;
            this->button1->Location = System::Drawing::Point(114, 442);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(111, 30);
            this->button1->TabIndex = 10;
            this->button1->Text = L"Log In";
            this->button1->UseVisualStyleBackColor = false;
            this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
            // 
            // buttonShowPassword
            // 
            this->buttonShowPassword->BackColor = System::Drawing::Color::Transparent;
            this->buttonShowPassword->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->buttonShowPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8));
            this->buttonShowPassword->ForeColor = System::Drawing::Color::SeaShell;
            this->buttonShowPassword->Location = System::Drawing::Point(298, 380);
            this->buttonShowPassword->Name = L"buttonShowPassword";
            this->buttonShowPassword->Size = System::Drawing::Size(30, 34);
            this->buttonShowPassword->TabIndex = 10;
            this->buttonShowPassword->Text = L"👁";
            this->buttonShowPassword->UseVisualStyleBackColor = false;
            this->buttonShowPassword->Click += gcnew System::EventHandler(this, &MyForm::buttonShowPassword_Click);
            // 
            // buttonRegister
            // 
            this->buttonRegister->BackColor = System::Drawing::SystemColors::ControlLightLight;
            this->buttonRegister->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->buttonRegister->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F));
            this->buttonRegister->ForeColor = System::Drawing::Color::LightSalmon;
            this->buttonRegister->Location = System::Drawing::Point(247, 481);
            this->buttonRegister->Name = L"buttonRegister";
            this->buttonRegister->Size = System::Drawing::Size(77, 35);
            this->buttonRegister->TabIndex = 11;
            this->buttonRegister->Text = L"Register";
            this->buttonRegister->UseVisualStyleBackColor = false;
            this->buttonRegister->Click += gcnew System::EventHandler(this, &MyForm::buttonRegister_Click);
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
            this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->ClientSize = System::Drawing::Size(329, 525);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->buttonShowPassword);
            this->Controls->Add(this->buttonRegister);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->textBox2);
            this->Controls->Add(this->checkBox1);
            this->Controls->Add(this->textBox1);
            this->DoubleBuffered = true;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->Name = L"MyForm";
            this->Text = L"LoginForm";
            this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
            // Optional: Add validation or tracking for username input if needed
        }

        System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
            if (String::IsNullOrEmpty(textBox1->Text) || String::IsNullOrEmpty(textBox2->Text)) {
                MessageBox::Show(L"Please fill in both fields!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            else if (!checkBox1->Checked) {
                MessageBox::Show(L"You must agree to the terms!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            else {
                // Retrieve email and password from form
                String^ email = this->textBox1->Text;
                String^ password = this->textBox2->Text;

                try {
                    // Database connection string
                    String^ connString = "Data Source=sqldatabasedm.database.windows.net;Initial Catalog=SampleDB;User ID=sqladmin;Password=sql123@gmail.com;Connect Timeout=60;Encrypt=True";
                    SqlConnection^ sqlConn = gcnew SqlConnection(connString);
                    sqlConn->Open();

                    // Query to check user credentials
                    String^ query = "SELECT * FROM Users WHERE Sender_Email = @Email AND Password = @Password";
                    SqlCommand^ cmd = gcnew SqlCommand(query, sqlConn);
                    cmd->Parameters->AddWithValue("@Email", email);
                    cmd->Parameters->AddWithValue("@Password", password);

                    SqlDataReader^ reader = cmd->ExecuteReader();

                    if (reader->Read()) {
                        // User found, fetch data
                        String^ userId = reader["Id"]->ToString();
                        String^ senderEmail = reader["Sender_Email"]->ToString();
                        String^ receiverEmail = reader["Receiver_Email"]->ToString();
                        String^ applicationKey = reader["Application_Key"]->ToString();

                        // Store the user information in the UserInfo header
                        UserInfo::User::SetUserInfo(userId, senderEmail, receiverEmail, applicationKey, password);

                        // Show the user details in a MessageBox for confirmation
                        String^ userInfo = "User Info:\n"
                            "User ID: " + userId + "\n" +
                            "Sender Email: " + senderEmail + "\n" +
                            "Receiver Email: " + receiverEmail + "\n" +
                            "Application Key: " + applicationKey;

                        MessageBox::Show(userInfo, L"Login Successful!");

                        // Open Selection Form with user credentials
                        LoginForm::SelectionForm^ selectionForm = gcnew LoginForm::SelectionForm(senderEmail, receiverEmail, applicationKey);
                        selectionForm->Show();
                        this->Hide();  // Hide login form
                    }
                    else {
                        MessageBox::Show(L"Invalid email or password!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                    }

                    sqlConn->Close();
                }
                catch (Exception^ e) {
                    MessageBox::Show("Error: " + e->Message);
                }
            }
        }



         System::Void RegisterFormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {
             this->Show(); // Show the login form again
         }

         System::Void buttonRegister_Click(System::Object^ sender, System::EventArgs^ e) {
             // Create and show the registration form   
             LoginForm::RegisterForm1^ registerForm1 = gcnew LoginForm::RegisterForm1();
             registerForm1->FormClosed += gcnew FormClosedEventHandler(this, &MyForm::RegisterFormClosed); // Handle form close event
             registerForm1->Show();
             this->Hide(); // Hide the login form
         }


        System::Void buttonShowPassword_Click(System::Object^ sender, System::EventArgs^ e) {
            System::Threading::Thread::Sleep(50);  // Add a small delay for smoother effect
            if (textBox2->PasswordChar == '*') {
                textBox2->PasswordChar = '\0';
                buttonShowPassword->Text = L"👁";  // Show icon
            }
            else {
                textBox2->PasswordChar = '*';
                buttonShowPassword->Text = L"🙈";  // Hide icon
            }
        }

        System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
            this->Opacity = 0; // Start fully transparent
            for (double i = 0; i <= 1; i += 0.05) {
                this->Opacity = i; // Gradually increase opacity
                System::Threading::Thread::Sleep(50); // Pause for a brief moment
            }
            this->Opacity = 1.0; // Default, Form not transparent

            // Consistent background color on startup
            buttonShowPassword->BackColor = System::Drawing::Color::Transparent;
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
        }

        System::Void button1_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
            button1->BackColor = System::Drawing::Color::DodgerBlue;
            button1->ForeColor = System::Drawing::Color::White;
        }

    private: System::Void button1_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
        button1->BackColor = System::Drawing::Color::LightCyan;
    }

    private: System::Void button1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
        button1->BackColor = System::Drawing::Color::RoyalBlue; // Darker color on click
    }

    private: System::Void button1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
        button1->BackColor = System::Drawing::Color::LightCyan; // Reset color when released
    }

    private: System::Void textBox1_Enter(System::Object^ sender, System::EventArgs^ e) {
        textBox1->BackColor = System::Drawing::Color::LightYellow; // Highlight on focus
    }

    private: System::Void textBox1_Leave(System::Object^ sender, System::EventArgs^ e) {
        textBox1->BackColor = System::Drawing::Color::DarkSalmon; // Reset color when focus is lost
    }

    private: System::Void textBox2_Enter(System::Object^ sender, System::EventArgs^ e) {
        textBox2->BackColor = System::Drawing::Color::LightYellow; // Highlight on focus
    }

    private: System::Void textBox2_Leave(System::Object^ sender, System::EventArgs^ e) {
        textBox2->BackColor = System::Drawing::Color::DarkSalmon; // Reset color when focus is lost
    }

    private: System::Void label5_Click(System::Object^ sender, System::EventArgs^ e) {
        // Handle terms and policy click, if needed
    }
    private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {
    }
};
}