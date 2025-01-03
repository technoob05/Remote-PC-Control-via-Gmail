#include "Client.h"
#include "ServerForm.h"
#pragma once

namespace LoginForm {

    using namespace System;
    using namespace System::Windows::Forms;

    public ref class SelectionForm : public System::Windows::Forms::Form
    {
    private:
        String^ senderEmail;
        String^ receiverEmail;
        String^ applicationKey;

    public:
        SelectionForm(String^ sender, String^ receiver, String^ appKey)
        {
            senderEmail = sender;
            receiverEmail = receiver;
            applicationKey = appKey;
            InitializeComponent();
        }

    protected:
        ~SelectionForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::Button^ clientButton;
        System::Windows::Forms::Button^ serverButton;
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->clientButton = (gcnew System::Windows::Forms::Button());
            this->serverButton = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // clientButton
            // 
            this->clientButton->Location = System::Drawing::Point(50, 50);
            this->clientButton->Name = L"clientButton";
            this->clientButton->Size = System::Drawing::Size(200, 50);
            this->clientButton->TabIndex = 0;
            this->clientButton->Text = L"Open Client";
            this->clientButton->UseVisualStyleBackColor = true;
            this->clientButton->Click += gcnew System::EventHandler(this, &SelectionForm::clientButton_Click);
            // 
            // serverButton
            // 
            this->serverButton->Location = System::Drawing::Point(50, 120);
            this->serverButton->Name = L"serverButton";
            this->serverButton->Size = System::Drawing::Size(200, 50);
            this->serverButton->TabIndex = 1;
            this->serverButton->Text = L"Open Server";
            this->serverButton->UseVisualStyleBackColor = true;
            this->serverButton->Click += gcnew System::EventHandler(this, &SelectionForm::serverButton_Click);
            // 
            // SelectionForm
            // 
            this->ClientSize = System::Drawing::Size(300, 250);
            this->Controls->Add(this->clientButton);
            this->Controls->Add(this->serverButton);
            this->Name = L"SelectionForm";
            this->Text = L"Select Form";
            this->Load += gcnew System::EventHandler(this, &SelectionForm::SelectionForm_Load);
            this->ResumeLayout(false);

        }
#pragma endregion

    private:
        void clientButton_Click(System::Object^ sender, System::EventArgs^ e) {
            // Open the Client form with parameters
            LoginForm::Client^ clientForm = gcnew LoginForm::Client(receiverEmail, applicationKey,senderEmail, 0);
            clientForm->Show();
            this->Hide(); // Hide selection form
        }

        void serverButton_Click(System::Object^ sender, System::EventArgs^ e) {
            // Open the Server form
            LoginForm::ServerForm^ serverForm = gcnew LoginForm::ServerForm();
            serverForm->Show();
            this->Hide(); // Hide selection form
        }
    private: System::Void SelectionForm_Load(System::Object^ sender, System::EventArgs^ e) {
    }
    };
}
