
#pragma once

namespace LoginForm {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Globalization;
    using namespace System::IO;
    using namespace System::Threading;
    using namespace System::Net;
    using namespace System::Net::Mail;
    using namespace System::Net::Sockets;
    using namespace System::Text;
    using namespace EAGetMail;

    public ref class Client : public System::Windows::Forms::Form
    {
    public:

        Client(String^ emailAddress, String^ appPassword, String^ specificSender, int checkInterval)
        {
            InitializeComponent();
            this->emailAddress = emailAddress;
            this->appPassword = appPassword;
            this->specificSender = specificSender;
            this->checkInterval = checkInterval;

            // Start the email processing in a background thread
            backgroundWorker = gcnew BackgroundWorker();
            backgroundWorker->DoWork += gcnew DoWorkEventHandler(this, &Client::OnDoWork);
            backgroundWorker->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &Client::OnRunWorkerCompleted);
            backgroundWorker->RunWorkerAsync();
        }

    protected:
        ~Client()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components;
        String^ emailAddress;
        String^ appPassword;
        String^ specificSender;
        int checkInterval;
        BackgroundWorker^ backgroundWorker;
        TextBox^ logTextBox;
        Button^ clearLogButton;
    private: System::Windows::Forms::Panel^ panel1;
           Button^ switchToServerButton;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Client::typeid));
            this->logTextBox = (gcnew System::Windows::Forms::TextBox());
            this->clearLogButton = (gcnew System::Windows::Forms::Button());
            this->switchToServerButton = (gcnew System::Windows::Forms::Button());
            this->panel1 = (gcnew System::Windows::Forms::Panel());
            this->SuspendLayout();
            // 
            // logTextBox
            // 
            this->logTextBox->Dock = System::Windows::Forms::DockStyle::Fill;
            this->logTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 10));
            this->logTextBox->Location = System::Drawing::Point(0, 0);
            this->logTextBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
            this->logTextBox->Multiline = true;
            this->logTextBox->Name = L"logTextBox";
            this->logTextBox->ReadOnly = true;
            this->logTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->logTextBox->Size = System::Drawing::Size(1132, 644);
            this->logTextBox->TabIndex = 0;
            this->logTextBox->TextChanged += gcnew System::EventHandler(this, &Client::logTextBox_TextChanged);
            // 
            // clearLogButton
            // 
            this->clearLogButton->Dock = System::Windows::Forms::DockStyle::Bottom;
            this->clearLogButton->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold));
            this->clearLogButton->Location = System::Drawing::Point(0, 644);
            this->clearLogButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
            this->clearLogButton->Name = L"clearLogButton";
            this->clearLogButton->Size = System::Drawing::Size(1132, 62);
            this->clearLogButton->TabIndex = 1;
            this->clearLogButton->Text = L"Clear Log";
            this->clearLogButton->UseVisualStyleBackColor = true;
            this->clearLogButton->Click += gcnew System::EventHandler(this, &Client::clearLogButton_Click);
            // 
            // switchToServerButton
            // 
            this->switchToServerButton->Dock = System::Windows::Forms::DockStyle::Bottom;
            this->switchToServerButton->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold));
            this->switchToServerButton->Location = System::Drawing::Point(0, 582);
            this->switchToServerButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
            this->switchToServerButton->Name = L"switchToServerButton";
            this->switchToServerButton->Size = System::Drawing::Size(1132, 62);
            this->switchToServerButton->TabIndex = 0;
            this->switchToServerButton->Text = L"Switch to Server";
            this->switchToServerButton->UseVisualStyleBackColor = true;
            this->switchToServerButton->Click += gcnew System::EventHandler(this, &Client::switchToServerButton_Click);
            // 
            // panel1
            // 
            this->panel1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"panel1.BackgroundImage")));
            this->panel1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->panel1->Location = System::Drawing::Point(608, -5);
            this->panel1->Name = L"panel1";
            this->panel1->Size = System::Drawing::Size(524, 580);
            this->panel1->TabIndex = 2;
            // 
            // Client
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1132, 706);
            this->Controls->Add(this->panel1);
            this->Controls->Add(this->switchToServerButton);
            this->Controls->Add(this->logTextBox);
            this->Controls->Add(this->clearLogButton);
            this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
            this->Name = L"Client";
            this->Text = L"Client Logs";
            this->ResumeLayout(false);
            this->PerformLayout();

        }

#pragma endregion

        void AppendLog(String^ message)
        {
            if (this->InvokeRequired)
            {
                this->Invoke(gcnew Action<String^>(this, &Client::AppendLog), message);
            }
            else
            {
                logTextBox->AppendText(message + Environment::NewLine);
            }
        }

        void OnDoWork(Object^ sender, DoWorkEventArgs^ e)
        {
            RunClient(emailAddress, appPassword, specificSender, checkInterval);
        }

        void OnRunWorkerCompleted(Object^ sender, RunWorkerCompletedEventArgs^ e)
        {
            AppendLog("Email processing finished.");
        }

        void clearLogButton_Click(System::Object^ sender, System::EventArgs^ e) {
            logTextBox->Clear();
        }

        // Method declarations
        void StartApplication(String^ emailAddress, String^ appPassword, String^ specificSender, int checkInterval);
        String^ GenerateFileName(int sequence);
        Imap4Folder^ FindInboxFolder(array<Imap4Folder^>^ folders);
        bool IsEmailProcessed(String^ messageId, String^ processedEmailsFile);
        void SaveProcessedEmail(String^ messageId, String^ processedEmailsFile);
        bool IsEmailFromSpecificSender(EAGetMail::Mail^ oMail, String^ specificSender);
        void ReceiveImage(NetworkStream^ stream);
        void SendEmail(String^ recipient, String^ subject, String^ body, String^ attachmentPath, String^ Credent, String^ app_password);
        array<String^>^ LoginForm::Client::ExtractEmailDetails(String^ emailBody);
        bool LoginForm::Client::HandleVerification(NetworkStream^ stream);
        void handleResponse(String^ command, NetworkStream^ stream, String^ recipientEmail, String^ Credent, String^ app_password);
        void RunClient(String^ emailAddress, String^ appPassword, String^ specificSender, int checkInterval);

    private:
        System::Void logTextBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
        }
        // Add the event handler

        // Event handler declaration
    private: System::Void switchToServerButton_Click(System::Object^ sender, System::EventArgs^ e);
    };

}