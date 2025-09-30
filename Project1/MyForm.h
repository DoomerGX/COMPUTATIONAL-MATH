#pragma once
#include<string>
#include "MyForm1.h"
namespace Project1 {
	using namespace std;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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

	private: System::Windows::Forms::TextBox^ inputBox;
	private: System::Windows::Forms::Button^ Generate;
	private: System::Windows::Forms::Label^ label1;
	protected: int value = 0;

	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->Generate = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// Generate
			// 
			this->Generate->BackColor = System::Drawing::Color::MediumSeaGreen;
			this->Generate->ForeColor = System::Drawing::Color::Snow;
			this->Generate->Location = System::Drawing::Point(374, 383);
			this->Generate->Name = L"Generate";
			this->Generate->Size = System::Drawing::Size(378, 70);
			this->Generate->TabIndex = 0;
			this->Generate->Text = L"Generate";
			this->Generate->UseVisualStyleBackColor = false;
			this->Generate->Click += gcnew System::EventHandler(this, &MyForm::Generate_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"MingLiU_HKSCS-ExtB", 18, System::Drawing::FontStyle::Bold));
			this->label1->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->label1->Location = System::Drawing::Point(284, 186);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(621, 24);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Enter number (values ought be between [8 - 16])";
			// 
			// inputBox
			// 
			this->inputBox = (gcnew System::Windows::Forms::TextBox());
			this->inputBox->Location = System::Drawing::Point(450, 300);
			this->inputBox->Size = System::Drawing::Size(240, 30);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::DarkSlateGray;
			this->ClientSize = System::Drawing::Size(1220, 611);
			this->Controls->Add(this->inputBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Generate);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion
	private: System::Void Generate_Click(System::Object^ sender, System::EventArgs^ e) {
		try {
			value = Convert::ToInt32(inputBox->Text);

			if (value >= 8 && value <= 16) {
				MyForm1^ form = gcnew MyForm1(value);
				form->Show();
				this->Hide();
			}
			else {
				MessageBox::Show("Enter a valid value.");
			}
		}
		catch (FormatException^) {
			MessageBox::Show("Enter a valid value.");
		}
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
