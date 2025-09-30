#pragma once

namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm1 : public System::Windows::Forms::Form
	{
	public:
		MyForm1(void)
		{
			InitializeComponent();
		}

		MyForm1(int v)
		{
			InitializeComponent();
			receivedValue = v;
			positions = gcnew cli::array<Point>(receivedValue);
			highLightedLine = gcnew cli::array<bool>(receivedValue);
		}

	protected:
		~MyForm1()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		int receivedValue;
		Random^ rand = gcnew Random();

		cli::array<Point>^ positions;
		cli::array<bool>^ highLightedLine;

		System::Windows::Forms::Button^ button1;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::MediumAquamarine;
			this->button1->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->button1->Location = System::Drawing::Point(430, 33);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(320, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"APPLY ALGORITHM";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm1::button1_Click);
			// 
			// MyForm1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::DarkSlateGray;
			this->ClientSize = System::Drawing::Size(1220, 611);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm1";
			this->Text = L"MyForm1";
			this->ResumeLayout(false);

		}
#pragma endregion

	protected:
		virtual void OnPaint(PaintEventArgs^ e) override {
			Form::OnPaint(e);

			Graphics^ g = e->Graphics;
			Pen^ pen = gcnew Pen(Color::White, 2);
			Pen^ arrowPen = gcnew Pen(Color::White, 2);
			arrowPen->CustomEndCap = gcnew System::Drawing::Drawing2D::AdjustableArrowCap(5, 5);

			int cx = this->ClientSize.Width / 2;
			int cy = this->ClientSize.Height / 2;
			int nodeRadius = 10;

			// Generate positions only once (if empty)
			for (int i = 0; i < receivedValue; i++) {
				if (positions[i].IsEmpty) {
					int x = rand->Next(100, cx + 200);
					int y = rand->Next(100, cy + 200);
					positions[i] = Point(x, y);
				}
			}

			// Draw edges
			for (int i = 0; i < receivedValue; i++) {
				int j = (i + 1) % receivedValue;
				Pen^ p = (highLightedLine != nullptr && highLightedLine[i])
					? gcnew Pen(Color::OrangeRed, 2)
					: arrowPen;

				g->DrawLine(p, positions[i].X, positions[i].Y,
					positions[j].X, positions[j].Y);
			}

			// Draw nodes
			for (int i = 0; i < receivedValue; i++) {
				Rectangle rect(positions[i].X - nodeRadius, positions[i].Y - nodeRadius,
					2 * nodeRadius, 2 * nodeRadius);
				g->FillEllipse(Brushes::LightBlue, rect);
				g->DrawEllipse(pen, rect);

				// Node label
				String^ label = i.ToString();
				g->DrawString(label, gcnew System::Drawing::Font("Arial", 10),
					Brushes::Black,
					positions[i].X - 10, positions[i].Y - 10);
			}
		}

	private:
		//WARNING U GUYS HAVE TO INCORPORATE THE ALGORITHM,THIS VERSION OF THE HIGLIGHTING IS JUST A RANDOMIZED, U GUYS HAVE TO IMPLEMENT IT :)
		System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
			// Randomly set which lines are highlighted
			for (int i = 0; i < receivedValue; i++) {
				highLightedLine[i] = (rand->Next(0, 2) == 0);
			}

			// Force redraw
			this->Invalidate();
		}
	};
}
