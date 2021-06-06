

namespace ui {

	class colorpicker : public object {

	public:

	

		colorpicker ( object * parent, ImColor& color );

		ImColor progress_color = ImColor(255, 255, 255, 255);

		bool modifying_picker = false;
		bool modifying_alpha = false;
		bool modififing_hue = false;
		bool open = false;
		ImVec2 bb_min;
		ImVec2 bb_max;

		ImVec2 picker_bb_min;
		ImVec2 picker_bb_max;

		ImVec2 hue_bb_min;
		ImVec2 hue_bb_max;

		ImVec2 alpha_bb_min;
		ImVec2 alpha_bb_max;


		ImVec2 text_bb_min;
		ImVec2 text_bb_max;

		ImVec2 copy_bb_min;
		ImVec2 copy_bb_max;

		ImVec2 paste_bb_min;
		ImVec2 paste_bb_max;

		float hue_val = 0.f;
		ImVec2 picker_cursor;
		ImColor * color;
		float alpha = 1.f;

		float  colHSV [ 3 ];

		ImColor get_color ( );

		ImColor predicted_color ( );

		float width = 160;
		float height = 150;

		std::string hex;
		float picker_width = 128;
		float picker_height = 128;


		void draw ( ) override;
		unsigned long create_hex ( int r, int g, int b );
		void picker_gradient ( );
		void handle ( ) override;
		
		std::string get_clipboard ( );

		void clipboard_copy ( );

		void update ( ) override;
		void handle_mouse_input ( ) override;

		void handle_text ( );

	};
}
