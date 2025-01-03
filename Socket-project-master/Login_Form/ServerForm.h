
#pragma once
namespace LoginForm {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Net::Sockets;
    using namespace System::Drawing;

    public ref class ServerForm : public System::Windows::Forms::Form
    {
    public:
        ServerForm()
        {
            InitializeComponent();

        }
        
    protected:
        ~ServerForm()
        {
            if (components)
            {
                delete components;
            }
        }
      


    private:
        // GUI components
        System::Windows::Forms::TextBox^ textBoxServerOutput;
        System::Windows::Forms::Button^ buttonStartServer;
        System::Windows::Forms::Button^ buttonStopServer;
        System::Windows::Forms::Label^ labelStatus;
        System::Windows::Forms::PictureBox^ pictureBoxStatus;
        System::Windows::Forms::CheckBox^ checkBoxDarkMode;  // Dark Mode CheckBox
        System::Windows::Forms::GroupBox^ groupBoxControls; // GroupBox for buttons

        System::ComponentModel::Container^ components;
        // Add this to the private section of your ServerForm class
        Button^ switchToClientButton;

        void InitializeComponent(void)
        {
            this->textBoxServerOutput = (gcnew System::Windows::Forms::TextBox());
            this->buttonStartServer = (gcnew System::Windows::Forms::Button());
            this->buttonStopServer = (gcnew System::Windows::Forms::Button());
            this->labelStatus = (gcnew System::Windows::Forms::Label());
            this->pictureBoxStatus = (gcnew System::Windows::Forms::PictureBox());
            this->checkBoxDarkMode = (gcnew System::Windows::Forms::CheckBox());
            this->groupBoxControls = (gcnew System::Windows::Forms::GroupBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxStatus))->BeginInit();
            this->groupBoxControls->SuspendLayout();

            this->switchToClientButton = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
   // switchToClientButton
   // 
            this->switchToClientButton->Dock = System::Windows::Forms::DockStyle::Bottom;
            this->switchToClientButton->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold));
            this->switchToClientButton->Location = System::Drawing::Point(0, 438);
            this->switchToClientButton->Name = L"switchToClientButton";
            this->switchToClientButton->Size = System::Drawing::Size(600, 50);
            this->switchToClientButton->Text = L"Switch to Client";
            this->switchToClientButton->UseVisualStyleBackColor = true;
            this->switchToClientButton->Click += gcnew System::EventHandler(this, &ServerForm::switchToClientButton_Click);
            // Add the new button to the controls
            this->Controls->Add(this->switchToClientButton);

            // 
            // textBoxServerOutput
            // 
            this->textBoxServerOutput->BackColor = System::Drawing::Color::AliceBlue;
            this->textBoxServerOutput->Font = (gcnew System::Drawing::Font(L"Consolas", 14));
            this->textBoxServerOutput->Location = System::Drawing::Point(7, 8);
            this->textBoxServerOutput->Margin = System::Windows::Forms::Padding(2);
            this->textBoxServerOutput->Multiline = true;
            this->textBoxServerOutput->Name = L"textBoxServerOutput";
            this->textBoxServerOutput->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->textBoxServerOutput->Size = System::Drawing::Size(601, 310);
            this->textBoxServerOutput->TabIndex = 0;
            this->textBoxServerOutput->TextChanged += gcnew System::EventHandler(this, &ServerForm::textBoxServerOutput_TextChanged_2);
            // 
            // buttonStartServer
            // 
            this->buttonStartServer->BackColor = System::Drawing::Color::LightGreen;
            this->buttonStartServer->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->buttonStartServer->Location = System::Drawing::Point(8, 24);
            this->buttonStartServer->Margin = System::Windows::Forms::Padding(2);
            this->buttonStartServer->Name = L"buttonStartServer";
            this->buttonStartServer->Size = System::Drawing::Size(112, 49);
            this->buttonStartServer->TabIndex = 0;
            this->buttonStartServer->Text = L"Start Server 🟢";
            this->buttonStartServer->UseVisualStyleBackColor = false;
            this->buttonStartServer->Click += gcnew System::EventHandler(this, &ServerForm::buttonStartServer_Click);
            // 
            // buttonStopServer
            // 
            this->buttonStopServer->BackColor = System::Drawing::Color::Tomato;
            this->buttonStopServer->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->buttonStopServer->Location = System::Drawing::Point(128, 24);
            this->buttonStopServer->Margin = System::Windows::Forms::Padding(2);
            this->buttonStopServer->Name = L"buttonStopServer";
            this->buttonStopServer->Size = System::Drawing::Size(112, 49);
            this->buttonStopServer->TabIndex = 1;
            this->buttonStopServer->Text = L"Stop Server 🔴";
            this->buttonStopServer->UseVisualStyleBackColor = false;
            this->buttonStopServer->Click += gcnew System::EventHandler(this, &ServerForm::buttonStopServer_Click);
            // 
            // labelStatus
            // 
            this->labelStatus->Font = (gcnew System::Drawing::Font(L"Arial", 14, System::Drawing::FontStyle::Bold));
            this->labelStatus->ForeColor = System::Drawing::Color::Gray;
            this->labelStatus->Location = System::Drawing::Point(8, 325);
            this->labelStatus->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->labelStatus->Name = L"labelStatus";
            this->labelStatus->Size = System::Drawing::Size(450, 24);
            this->labelStatus->TabIndex = 2;
            this->labelStatus->Text = L"Status: Server is stopped";
            this->labelStatus->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            // 
            // pictureBoxStatus
            // 
            this->pictureBoxStatus->BackColor = System::Drawing::Color::Gray;
            this->pictureBoxStatus->Location = System::Drawing::Point(465, 325);
            this->pictureBoxStatus->Margin = System::Windows::Forms::Padding(2);
            this->pictureBoxStatus->Name = L"pictureBoxStatus";
            this->pictureBoxStatus->Size = System::Drawing::Size(38, 41);
            this->pictureBoxStatus->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
            this->pictureBoxStatus->TabIndex = 1;
            this->pictureBoxStatus->TabStop = false;
            // 
            // checkBoxDarkMode
            // 
            this->checkBoxDarkMode->AutoSize = true;
            this->checkBoxDarkMode->Font = (gcnew System::Drawing::Font(L"Arial", 12));
            this->checkBoxDarkMode->Location = System::Drawing::Point(12, 455);
            this->checkBoxDarkMode->Margin = System::Windows::Forms::Padding(2);
            this->checkBoxDarkMode->Name = L"checkBoxDarkMode";
            this->checkBoxDarkMode->Size = System::Drawing::Size(105, 22);
            this->checkBoxDarkMode->TabIndex = 0;
            this->checkBoxDarkMode->Text = L"Dark Mode";
            this->checkBoxDarkMode->UseVisualStyleBackColor = true;
            this->checkBoxDarkMode->CheckedChanged += gcnew System::EventHandler(this, &ServerForm::checkBoxDarkMode_CheckedChanged);
            // 
            // groupBoxControls
            // 
            this->groupBoxControls->Controls->Add(this->buttonStartServer);
            this->groupBoxControls->Controls->Add(this->buttonStopServer);
            this->groupBoxControls->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold));
            this->groupBoxControls->Location = System::Drawing::Point(12, 361);
            this->groupBoxControls->Margin = System::Windows::Forms::Padding(2);
            this->groupBoxControls->Name = L"groupBoxControls";
            this->groupBoxControls->Padding = System::Windows::Forms::Padding(2);
            this->groupBoxControls->Size = System::Drawing::Size(255, 81);
            this->groupBoxControls->TabIndex = 3;
            this->groupBoxControls->TabStop = false;
            this->groupBoxControls->Text = L"Server Control";
            this->groupBoxControls->Enter += gcnew System::EventHandler(this, &ServerForm::groupBoxControls_Enter);
            // 
            // ServerForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(600, 488);
            this->Controls->Add(this->checkBoxDarkMode);
            this->Controls->Add(this->pictureBoxStatus);
            this->Controls->Add(this->labelStatus);
            this->Controls->Add(this->groupBoxControls);
            this->Controls->Add(this->textBoxServerOutput);
            this->Margin = System::Windows::Forms::Padding(2);
            this->Name = L"ServerForm";
            this->Text = L"Server Control Panel";
            this->Load += gcnew System::EventHandler(this, &ServerForm::ServerForm_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxStatus))->EndInit();
            this->groupBoxControls->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }

        // Event handlers
    private: System::Void buttonStartServer_Click(System::Object^ sender, System::EventArgs^ e);
    private: System::Void buttonStopServer_Click(System::Object^ sender, System::EventArgs^ e);
    private: System::Void checkBoxDarkMode_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
    public: void UpdateCommunicationLog(String^ message);
    public: void StartServer(); // Ensure StartServer is implemented
    private: System::Void ServerForm_Load(System::Object^ sender, System::EventArgs^ e) {}
    private: System::Void textBoxServerOutput_TextChanged(System::Object^ sender, System::EventArgs^ e) {}
    private: System::Void pictureBoxStatus_Click(System::Object^ sender, System::EventArgs^ e) {}
    private: System::Void textBoxServerOutput_TextChanged_1(System::Object^ sender, System::EventArgs^ e) {}
    private: System::Void groupBoxControls_Enter(System::Object^ sender, System::EventArgs^ e) {
    }
    
    
    //public: void SendFileToClient(TcpClient^ client, String^ filePath);





private: System::Void textBoxServerOutput_TextChanged_2(System::Object^ sender, System::EventArgs^ e) {
}
       // Add the event handler
private: System::Void switchToClientButton_Click(System::Object^ sender, System::EventArgs^ e);

};



}
