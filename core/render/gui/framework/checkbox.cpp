#pragma once
#include "../includes.h"

#include <Windows.h>
#include <iostream>
#include <cmath>


namespace ui {

	checkbox::checkbox(SecureString& text, object* parent, bool& _value, ImColor& color, config_manager::key_bind_item& key_bind_item) {
		this->title = &text;
		this->value = &_value;
		key_bind_controller = &key_bind_item;


		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = checkbox_element;


		this->parrent->add_children(this);
		last_value = *value;
		new key_bind_muie(this, key_bind_controller);
		new colorpicker(this, color);
	}

	checkbox::checkbox(SecureString& text, object* parent, bool& _value, config_manager::key_bind_item* key_bind_item) {

		this->title = &text;
		this->value = &_value;
		key_bind_controller = key_bind_item;


		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = checkbox_element;


		this->parrent->add_children(this);
		last_value = *value;
		new key_bind_muie(this, key_bind_item);
	}

	checkbox::checkbox(SecureString& text, object* parent, bool& _value, ImColor& color) {


		this->title = &text;
		this->value = &_value;


		this->parrent = parent;
		this->renderer = this->parrent->renderer;
		this->type = checkbox_element;


		this->parrent->add_children(this);
		last_value = *value;
		new colorpicker(this, color);

	}
	void checkbox::draw() {

		this->handle();

	//	this->renderer->AddRectFilled (this->mins, this->maxs, ImColor ( 255, 0, 255, 255 ), 0.f );

		float progress_alpha = ImGui::GetTime() - hover_start_time;  progress_alpha = std::clamp(progress_alpha * 3.5f, 0.f, 1.f);


		int alpha;

		if (this->hovering) {
			alpha = (int)std::lerp((max_alpha * 0.05f), max_alpha * 0.274f, progress_alpha);

		}
		else {
			alpha = (int)std::lerp((max_alpha * 0.05f), max_alpha * 0.274f, 1.f - progress_alpha);

		}

		float progress = (float)(this->animation_step - this->bb_min.x) / (float)(this->bb_max.x - this->bb_min.x);


		this->renderer->AddRectFilled(ImVec2(this->bb_min.x - 9.f, this->bb_min.y), ImVec2(this->bb_max.x + 9.f, this->bb_max.y), ImColor(46, 49, 52, (int)(max_alpha * 0.66f)), 8.5f);



		this->renderer->AddRect(ImVec2(this->bb_min.x - 9.f, this->bb_min.y), ImVec2(this->bb_max.x + 9.f, this->bb_max.y), ImColor(255, 255, 255, alpha), 8.5f);
		ImVec2 center;
		center.x = (bb_min.x + bb_max.x) / 2.f;
		center.y = (bb_min.y + bb_max.y) / 2.f;



		//25, 125, 123, 255

		int r = static_cast<int>(std::lerp(max_alpha, (int)(max_alpha * 0.09f), progress));
		int g = static_cast<int>(std::lerp(max_alpha, (int)(max_alpha * 0.4901f), progress));
		int b = static_cast<int>(std::lerp(max_alpha, (int)(max_alpha * 0.482f), progress));



		ImColor progress_color = ImColor(r, g, b, max_alpha);

		this->renderer->AddShadowCircle(ImVec2(this->animation_step, (bb_min.y + bb_max.y) / 2.f), 8.f, ImColor(0, 0, 0, (int)(max_alpha * 0.49f)), 3.f, ImVec2(0, 0));
		this->renderer->AddCircleFilled(ImVec2(this->animation_step, (bb_min.y + bb_max.y) / 2.f), 6.f, progress_color);


		ImVec2 middle = ImVec2((this->mins.x + this->maxs.x) / 2.f, (this->maxs.y + this->mins.y) / 2.f);

		middle.y -= ImGui::CalcTextSize(this->title->str.data(), 13.f, ui::font_widgets).y / 2.f;
		float offset_x = 1.2f;
		float offset_y = 1.2f;


		int alpha_title;

		if (this->hovering) {
			alpha_title = (int)std::lerp((int)(max_alpha * 0.88f), max_alpha, progress_alpha);

		}
		else {
			alpha_title = (int)std::lerp((int)(max_alpha * 0.88f), max_alpha, 1.f - progress_alpha);

		}

		this->renderer->AddText(ui::font_widgets, 13.f, ImVec2(this->mins.x, middle.y), ImColor(255, 255, 255, alpha_title), this->title->str.data());



		for (size_t i = this->children.size() - 1; i != (size_t)-1; i--)
			this->children.at(i)->draw();

		draw_keybind();
	}
	void checkbox::handle_mouse_input() {

		if (!can_focus())
			return;



		auto mouse_pos = ui::get_cursor();
		//ImVec2 ( this->bb_min.x - 9.f, this->bb_min.y ), ImVec2 ( this->bb_max.x + 9.f, this->bb_max.y )
		this->hovering = (mouse_pos.x > this->bb_min.x - 9.f && mouse_pos.y > this->bb_min.y && mouse_pos.x < this->bb_max.x + 9.f && mouse_pos.y < this->bb_max.y);
	}
	void checkbox::draw_keybind() {
		if (!this->key_bind_open)
			return;


	}
	void checkbox::handle() {

		handle_mouse_input();



		if (this->should_reset) {
			this->animation_step = this->bb_min.x;
			this->should_reset = false;
		}
		if (this->should_reanimate) {
			this->in_animation = true;
			this->updated_last_time = true;
			this->should_reanimate = false;
		}



		if (this->hovering && key_released(VK_RBUTTON)) {
			this->key_bind_open = !this->key_bind_open;
		}

		if (this->hovering && key_pressed(VK_LBUTTON) && !this->in_animation) {
			*this->value = !*this->value;
			ui::game_logs.push_back(new game_log("Denis bought a scout 123$."));
		}
		if (*this->value != this->old_value) {


			this->updated_last_time = true;

			this->in_animation = true;

			this->old_value = *this->value;
		}


		if (this->in_animation) {

			if (!*this->value) {
				float speed = (this->animation_step - this->bb_min.x) / (this->bb_max.x - this->bb_min.x);
				this->animation_step -= ((1000.0 / (double)ImGui::GetIO().Framerate) / 5.0 * (speed > 0.1f ? speed : 1.f));
				if (this->animation_step < this->bb_min.x) {
					this->in_animation = false;
					this->animation_step = this->bb_min.x;

					if (!this->updated_last_time)
						*this->value = !*this->value;

					if (this->updated_last_time)
						this->updated_last_time = false;
				}
			}
			else {
				float speed = 1.0f - (this->animation_step - this->bb_min.x) / (this->bb_max.x - this->bb_min.x);
				this->animation_step += ((1000.0 / (double)ImGui::GetIO().Framerate) / 5.0 * (speed > 0.1f ? speed : 1.f));
				if (this->animation_step > this->bb_max.x) {
					this->in_animation = false;
					this->animation_step = this->bb_max.x;

					if (!this->updated_last_time)
						*this->value = !*this->value;

					if (this->updated_last_time)
						this->updated_last_time = false;
				}
			}
		}

		if (!this->in_animation) {
			if (this->hovering != this->was_hovering)
				hover_start_time = ImGui::GetTime();



			this->was_hovering = this->hovering;
		}
	}
	void checkbox::update() {

		if (!this->value)
			return;

		if (this->index > 0) {
			auto& back = this->parrent->children.at(this->index - 1);
			this->mins = ImVec2(this->parrent->mins.x + this->parrent->padding, back->maxs.y + 8);
			this->maxs = ImVec2(this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20);


			this->bb_min = ImVec2(this->maxs.x - 30, this->mins.y);
			this->bb_max = ImVec2(this->maxs.x - 10, this->maxs.y);

			if (*this->value)
				this->animation_step = this->bb_max.x;
			else
				this->animation_step = this->bb_min.x;
		}
		else {
			bool is_parent_panel = this->parrent->type == panel_element || this->parrent->type == panel_cotainer_element;
			if (is_parent_panel) {

				/*if ( this->parrent->parrent && this->parrent->parrent->children.size() > 0 ) {
					auto previous_element = this->parrent->parrent->children.at ( this->parrent->index  - 1);

					this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, previous_element->maxs.y + 8 );
				}
				else {
					this->mins = ImVec2 ( this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y + 8);
				}*/
				this->mins = ImVec2(this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y);
				this->maxs = ImVec2(this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20);


				this->bb_min = ImVec2(this->maxs.x - 30, this->mins.y);
				this->bb_max = ImVec2(this->maxs.x - 10, this->maxs.y);
			}
			else {
				this->mins = ImVec2(this->parrent->mins.x + this->parrent->padding, this->parrent->mins.y + 40);
				this->maxs = ImVec2(this->parrent->maxs.x - this->parrent->padding, this->mins.y + 20);


				this->bb_min = ImVec2(this->maxs.x - 30, this->mins.y);
				this->bb_max = ImVec2(this->maxs.x - 10, this->maxs.y);
			}

			if (*this->value)
				this->animation_step = this->bb_max.x;
			else
				this->animation_step = this->bb_min.x;

		}

		this->key_bind_mins.x = this->bb_min.x + 5.f;
		this->key_bind_mins.y = this->bb_min.y + 5.f;

		this->key_bind_maxs.x = this->key_bind_mins.x + this->key_bind_width;
		this->key_bind_maxs.y = this->key_bind_mins.y + this->key_bind_height;


		for (auto& child : this->children)
			child->update();
	}
}