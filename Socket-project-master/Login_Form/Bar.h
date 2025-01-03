#pragma once

namespace LoginForm {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Text::RegularExpressions;
    using namespace System::Data::SqlClient; // SQL Client namespace
    using namespace System::Drawing::Drawing2D; // For LinearGradientBrush

    // Custom ProgressBar class
    public ref class CustomProgressBar : public System::Windows::Forms::ProgressBar {
    public:
        CustomProgressBar() {
            this->SetStyle(ControlStyles::UserPaint, true);
            this->SetStyle(ControlStyles::AllPaintingInWmPaint, true);
            this->SetStyle(ControlStyles::DoubleBuffer, true);
        }

    protected:
        virtual void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override {
            ProgressBar::OnPaint(e);
            if (this->Value > 0) {
                System::Drawing::Color startColor;
                System::Drawing::Color endColor;

                if (this->Value < 50) {
                    startColor = System::Drawing::Color::Red; // Weak
                    endColor = System::Drawing::Color::Orange;
                }
                else if (this->Value < 75) {
                    startColor = System::Drawing::Color::Yellow; // Medium
                    endColor = System::Drawing::Color::GreenYellow;
                }
                else {
                    startColor = System::Drawing::Color::Green; // Strong
                    endColor = System::Drawing::Color::LimeGreen;
                }

                int width = (int)((this->Width * this->Value) / this->Maximum);
                System::Drawing::Drawing2D::LinearGradientBrush^ brush = gcnew System::Drawing::Drawing2D::LinearGradientBrush(
                    System::Drawing::Rectangle(0, 0, width, this->Height),
                    startColor,
                    endColor,
                    0.0f
                );
                e->Graphics->FillRectangle(brush, 0, 0, width, this->Height);
                delete brush;
            }
        }


    };

}
