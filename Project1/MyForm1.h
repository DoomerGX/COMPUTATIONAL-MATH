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
			this->DoubleBuffered = true; // TO AVOID FLICKERING
			this->ResizeRedraw = true; // REDRAW WHEN RESIZING
		}

		MyForm1(int v)
		{
			InitializeComponent();
			receivedValue = v;
			maxFlow = 0;
			positions = gcnew cli::array<Point>(receivedValue);
			highLightedLine = gcnew cli::array<bool>(receivedValue);
			capacity = gcnew cli::array<cli::array<int>^>(receivedValue);
			flow = gcnew cli::array<cli::array<int>^>(receivedValue);
			full = gcnew cli::array<cli::array<bool>^>(receivedValue); 
			variation = gcnew cli::array<int>(receivedValue);

			for (int i = 0; i < receivedValue; i++) {
				capacity[i] = gcnew cli::array<int>(receivedValue);
				flow[i] = gcnew cli::array<int>(receivedValue);
				full[i] = gcnew cli::array<bool>(receivedValue);
				for (int j = 0; j < receivedValue; j++) {
					capacity[i][j] = 0;
					flow[i][j] = 0;
					full[i][j] = false;
				}
			}

			// GENERATE GRAPH
			for (int i = 0; i < receivedValue - 1; i++) {
				for (int j = i + 1; j < receivedValue; j++) {
					if (rand->Next(0, 3) == 0) { // PROBABILITY TO CREATE A NEW LINE
						int cap = rand->Next(5, 20);
						capacity[i][j] = cap; 
						flow[i][j] = 0;      
						full[i][j] = false;  
					}
				}
			}
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
		int maxFlow;
		Random^ rand = gcnew Random();

		// MATRIX OF FLOW AND CAPACITY
		cli::array<cli::array<int>^>^ capacity;
		cli::array<cli::array<int>^>^ flow;
		//MAX CAPACITY CHECKER POR ARISTA
		cli::array<cli::array<bool>^>^ full;
		//VARITION ARRAY
		cli::array<int>^ variation;

		// NODES´S POSITIONS
		cli::array<Point>^ positions;

		// TO HIGHLIGHT THE LINES
		cli::array<bool>^ highLightedLine;
		System::Windows::Forms::Label^ MaxFlowL;
		System::Windows::Forms::Button^ button1;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->MaxFlowL = (gcnew System::Windows::Forms::Label());
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
			// MaxFlow LABEL
			// 
			this->MaxFlowL->BackColor = System::Drawing::Color::MediumAquamarine;
			this->MaxFlowL->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->MaxFlowL->Location = System::Drawing::Point(430, 62);
			this->MaxFlowL->Visible = false;
			this->MaxFlowL->Name = L"MaxFlowL";
			this->MaxFlowL->Size = System::Drawing::Size(320, 23);
			this->MaxFlowL->TabIndex = 1;
			this->MaxFlowL->Text = maxFlow.ToString();
			// 
			// MyForm1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::DarkSlateGray;
			this->ClientSize = System::Drawing::Size(1500, 800);
			this->Controls->Add(this->MaxFlowL);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm1";
			this->Text = L"MyForm1";
			this->ResumeLayout(false);
		}
#pragma endregion

	protected:
		virtual void OnPaint(PaintEventArgs^ e) override {
			Form::OnPaint(e);

			if (this->DesignMode) return;

			Graphics^ g = e->Graphics;
			g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

			Pen^ pen = gcnew Pen(Color::White, 2);
			Pen^ arrowPen = gcnew Pen(Color::White, 2);
			arrowPen->CustomEndCap = gcnew System::Drawing::Drawing2D::AdjustableArrowCap(5, 5);

			int nodeRadius = 15;
			int minDistance = 100;

			// Useful area (avoid covering the controls above)
			int margin = 80;
			int topReserved = 110; // button + label
			int W = this->ClientSize.Width;
			int H = this->ClientSize.Height;

			// If there are no positions yet, generate random ones within the useful area
			for (int i = 0; i < receivedValue; i++) {
				if (positions[i].IsEmpty) {
					bool valid = false;
					Point candidate;
					int attempts = 0;
					while (!valid && attempts < 2000) {
						attempts++;
						int x = rand->Next(margin, Math::Max(margin + 1, W - margin));
						int y = rand->Next(topReserved, Math::Max(topReserved + 1, H - margin));
						candidate = Point(x, y);
						valid = true;
						for (int k = 0; k < i; k++) {
							int dx = candidate.X - positions[k].X;
							int dy = candidate.Y - positions[k].Y;
							double dist = Math::Sqrt((double)dx * dx + (double)dy * dy);
							if (dist < minDistance) { valid = false; break; }
						}
					}
					if (!valid) candidate = Point(margin + (i * 25) % (W - 2 * margin),
						topReserved + ((i * 35) % (H - topReserved - margin)));
					positions[i] = candidate;
				}
			}

			// --- CENTER + SCALING ---
			// actual bounding box
			int minX = Int32::MaxValue, minY = Int32::MaxValue, maxX = Int32::MinValue, maxY = Int32::MinValue;
			for (int i = 0; i < receivedValue; i++) {
				minX = Math::Min(minX, positions[i].X);
				minY = Math::Min(minY, positions[i].Y);
				maxX = Math::Max(maxX, positions[i].X);
				maxY = Math::Max(maxY, positions[i].Y);
			}
			int graphW = Math::Max(1, maxX - minX);
			int graphH = Math::Max(1, maxY - minY);

			// Scale to fit the useful rectangle (with margins)
			float availW = (float)(W - 2 * margin);
			float availH = (float)(H - topReserved - margin);
			float scale = 1.0f;
			// Only scale if the graphic is larger than the usable area
			if (graphW > availW || graphH > availH) {
				float sx = availW / (float)graphW;
				float sy = availH / (float)graphH;
				scale = Math::Min(sx, sy);
			}

			// We center: we take (minX,minY) to (0,0), scale, and move to the center of the useful area
			float drawW = graphW * scale;
			float drawH = graphH * scale;
			float offsetX = (W - drawW) / 2.0f - (minX * scale);
			float offsetY = (topReserved + (availH - drawH) / 2.0f) - (minY * scale);

			// We apply transformation to the graphics (first scale, then translate)
			g->TranslateTransform(offsetX, offsetY);
			g->ScaleTransform(scale, scale);

			// DRAW LINES
			for (int i = 0; i < receivedValue; i++) {
				for (int j = 0; j < receivedValue; j++) {
					if (capacity[i][j] > 0) { // CHECKS IF ITS EMPTY
						Pen^ p;

						// IF ITS FULL THEN IT´LL BE DRAWNED IN GRAY
						if (full[i][j]) {
							p = gcnew Pen(Color::OrangeRed, 2);
						}
						else if (highLightedLine != nullptr && highLightedLine[i]) {
							p = gcnew Pen(Color::Gray, 2);
						}
						else {
							p = arrowPen;
						}

						// DRAW LINE
						g->DrawLine(p, positions[i], positions[j]);

						// DISTANCES COORDINATES
						int midX = (positions[i].X + positions[j].X) / 2;
						int midY = (positions[i].Y + positions[j].Y) / 2;

						// LABELS "capacity/flow"
						String^ label = capacity[i][j].ToString() + "/" + flow[i][j].ToString();
						g->DrawString(label,
							gcnew System::Drawing::Font("Arial", 10),
							Brushes::Black,
							midX, midY);
					}
				}
			}

			// DRAW NODES
			for (int i = 0; i < receivedValue; i++) {
				Rectangle rect(positions[i].X - nodeRadius, positions[i].Y - nodeRadius,
					2 * nodeRadius, 2 * nodeRadius);
				g->FillEllipse(Brushes::LightBlue, rect);
				g->DrawEllipse(pen, rect);

				// INDEX NODE
				String^ labelNumber = i.ToString();
				g->DrawString(labelNumber, gcnew System::Drawing::Font("Arial", 10),
					Brushes::Black,
					positions[i].X - 10, positions[i].Y - 10);
			}
		}

	private:
		bool IsSmaller(int a, int b) {
			return a < b;
		}
		//u is my initial path , t is my destiny. Parent allows us to save the data from which it was as an example
		//a->d, basically im copying it LOL
		//n is the amount of nodes
		bool Path(cli::array<cli::array<int>^>^ residual, int u, int t,
			cli::array<bool>^ visited, cli::array<int>^ parent, int n) {
			//it returns true because the path exists
			if (u == t) return true;
			visited[u] = true;

			for (int v = 0; v < n; v++) {
				//IF WE HAVENT VISITED THE PATH AND IT EXISTS , WE SAVE THE COORDINATES OF THE NODE THAT WE WERE
				if (!visited[v] && residual[u][v] > 0) {
					parent[v] = u;
					//WE´RE USING A RECURSIVE FUNCTION JUST TO ENSURE THAT IT EXISTS AND MARK IT AS A VISITED PATH
					if (Path(residual, v, t, visited, parent, n))
						return true;
				}
			}
			return false;
		}

		void FulkersonAlgorithm() {
			int n = receivedValue;
			int source = 0;         // INITIAL NODE
			int sink = n - 1;       // LAST NODE

			// WE COPY IF WE CAN REVERT THE PATHS, AS A SIMPLE WAY TO EXPLAIN IT, A RESIDUAL PATH IS A PATH THAT HAS BEEN EXCEEDED AND IT SHOULD BE REVERTED
			cli::array<cli::array<int>^>^ residual = gcnew cli::array<cli::array<int>^>(n);
			for (int i = 0; i < n; i++) {
				residual[i] = gcnew cli::array<int>(n);
				for (int j = 0; j < n; j++) {
					residual[i][j] = capacity[i][j];
				}
			}

			cli::array<int>^ parent = gcnew cli::array<int>(n);
			maxFlow = 0;

			// WHILE THERE´S A GROWING PATH 
			while (true) {
				cli::array<bool>^ visited = gcnew cli::array<bool>(n);
				for (int i = 0; i < n; i++) { visited[i] = false; parent[i] = -1; }

				if (!Path(residual, source, sink, visited, parent, n))
					break; // NOT MORE GROWING PATHS 

				// TO FIND THE MIMINUM FLOW
				int pathFlow = Int32::MaxValue;
				for (int v = sink; v != source; v = parent[v]) {
					int u = parent[v];
					pathFlow = Math::Min(pathFlow, residual[u][v]);
				}

				// UPDATE OF THE RESIDUAL NET
				for (int v = sink; v != source; v = parent[v]) {
					int u = parent[v];
					residual[u][v] -= pathFlow;
					residual[v][u] += pathFlow;

					// UPDATE FLOW
					flow[u][v] += pathFlow;
					highLightedLine[u] = true; // HIGHLIGHT IF THE PATH WAS USED
					if (flow[u][v] >= capacity[u][v]) {
						full[u][v] = true;
					}
				}

				// GET THE MAX FLOW
				maxFlow += pathFlow;
			}
		}

	private:
		System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
			FulkersonAlgorithm();
			this->Invalidate(); //REDRAW
			this->MaxFlowL->Text = "Max Flow: " + maxFlow.ToString();
			MaxFlowL->Visible = true;
		}
	};
}