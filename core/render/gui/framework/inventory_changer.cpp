#include <iostream>
#ifdef PREVIEW_D3D
#include "../includes.h"
//
#include "../../../../dependencies/utilities/base64.h"
#include "../../../../dependencies/utilities/crypt_str.h"
#endif

#include <shlobj.h>
#include <iostream>
#include <fstream>
#include  <iomanip>
#ifdef PREVIEW_D3D
#include "../../../../dependencies/utilities/json.hpp"
#include "../config.h"
#endif
#include <sstream>
#include <random>
#include <string>
#include <tchar.h>
#include <thread>
#include <d3dx9.h>
#include <mutex>
#include <atomic>

#ifdef PREVIEW_D3D

#else
#include "../../../features/features.hpp"
#include "../gui.h"
#include "../../../../dependencies/utilities/vpk_parser.h"
#endif


namespace ui {

	void image_queue::remove(int index) {
		//images_data.erase ( images_data.begin ( ) + index );
		//delete this;
	}

	ui::child_window* items_weapons_active = nullptr;
	ui::child_window* items_weapons_paints = nullptr;
	ui::child_window* items_weapons_list = nullptr;
	ui::child_window* item_painkit_settings = nullptr;

	std::queue<ui::image_queue*> images_data;


	std::mutex mutex_image_data;
	std::atomic<bool> done(false);
	void samp() {


	}
	void ui::handle_image_data() {


		static auto start_time = 0.f;


		int i = 0;
#ifdef PREVIEW_D3D
		while (!done.load()) {
#endif

			static float last_gpu_scan = 0.f;


			if (ImGui::GetTime() != -1.0 && ui::window_device) {


				if (!images_data.empty()) {
					if ((ImGui::GetTime() - last_gpu_scan) > 0.015f) {


						mutex_image_data.lock();
						auto& img = images_data.front();

						std::optional<c_vpk_entry> entry = static_cast<c_vpk_archive*>(img->archive)->get_file(img->location);

						if (entry) {

							std::optional<std::vector<uint8_t>> png_data = entry.value().get_data();
							if (png_data.has_value()) {
								//D3DXCreateTextureFromFileInMemory ( reinterpret_cast< IDirect3DDevice9 * >( ui::window_device ), ( char * ) png_data.value ( ).data ( ), png_data.value /( ).size ( ), img->image_buffer );

								//

								D3DXCreateTextureFromFileInMemoryEx(reinterpret_cast<IDirect3DDevice9*>(ui::window_device), (char*)png_data.value().data(), png_data.value().size(),
									200, 200, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, NULL, NULL, img->image_buffer);

							}
						}

						images_data.pop();

						mutex_image_data.unlock();


						last_gpu_scan = ImGui::GetTime();
					}



				}



			}
#ifdef PREVIEW_D3D
		}
#endif
	}

	void inventory_changer::on_start() {
		std::ifstream in("E:\\repos\\mDenis16\\getaxe-menu-framework\\Release\\skins.json"); /*this needs to be streamed from main network*/
		std::ifstream weapons_stream("E:\\SteamLibrary\\steamapps\\common\\Counter-Strike Global Offensive\\csgo\\scripts\\items\\csgo_weaponid_dumper-master\\Debug\\weapons.json");

		archive = new c_vpk_archive();
		static_cast<c_vpk_archive*>(archive)->load("E:/SteamLibrary/steamapps/common/Counter-Strike Global Offensive/csgo/pak01_dir.vpk");

		nlohmann::json json_weapons;
		weapons_stream >> json_weapons;

		nlohmann::json json;
		in >> json;

		nlohmann::json paintkit_display_names = json["paintkit_names"];
		nlohmann::json paintkit_ids = json["paintkit_ids"];
		//static int filtered_ids = { }
		int k = 0;
		for (auto it = json_weapons["data"].begin(); it != json_weapons["data"].end(); ++it, k++) {
			auto j = *it;

			auto  weap = new weapon(j["id"].get<int>());


			auto str = std::to_string(j["id"].get<int>());

			nlohmann::json paintkit_names = json["weapon_skins"][str]["paintkit_names"];
			nlohmann::json paintkit_rarities = json["weapon_skins"][str]["paintkit_rarities"];
			weap->weapon_name = j["weapon_name"].get<std::string>();
			weap->display_name = j["display_name"].get<std::string>();
			weap->index_in_list = k;
			weap->item_definition_index = j["id"].get<int>();

			for (size_t i = 0; i < paintkit_names.size(); i++) {
				auto  paint = new paintkit();
				paint->game_name = paintkit_names[i].get<std::string>();
				paint->rarity = paintkit_rarities[i].get<int>();
				paint->weapon_name = weap->weapon_name;
				paint->weapon_id = weap->item_definition_index;
				paint->display_name = paintkit_display_names[paint->game_name].get<std::string>();
				paint->id = paintkit_ids[paint->game_name].get<int>();
				paint->generate_image_path();

				weap->paintkits.emplace_back(paint);
			}

			this->weapons.emplace_back(weap);


		}

	}


	void inventory_changer::add_item_to_inventory(weapon* weap, paintkit* pkit) {
#ifdef PREVIEW_D3D

		std::string samp = "shit called";
		int i = 0;

		auto inv = new ui::inventory_item((ui::object*)items_weapons_active, pkit, this->archive, std::bind(&inventory_changer::painkit_modify, this, pkit));


#else
		static auto g_CSInventoryManager = *reinterpret_cast<CSInventoryManager**>(utilities::pattern_scan("client.dll", "B9 ?? ?? ?? ?? 8D 44 24 10 89 54 24 14") + 0x1);

		static auto local_inventory_offset = *reinterpret_cast<uintptr_t*>(utilities::pattern_scan("client.dll", "8B 8F ? ? ? ? 0F B7 C0 50") + 0x2);

		//auto LocalInventory = g_CSInventoryManager->GetLocalPlayerInventory ( );

		static int itm_id = 500;
		itm_id++;

		auto Item = g_CSInventoryManager->CreateEconItem();

		auto LocalInventory = *reinterpret_cast<CPlayerInventory**>(g_CSInventoryManager + local_inventory_offset);


		*Item->GetAccountID() = LocalInventory->GetSteamID();
		*Item->GetDefIndex() = pkit->weapon_id;
		*Item->GetItemID() = itm_id;
		*Item->GetInventory() = 1;
		*Item->GetFlags() = 0;
		*Item->GetOriginalID() = 0;
		//Item->AddSticker ( 0, 4, 0, 1, 1 );
		Item->SetStatTrak(150);
		Item->SetPaintKit(pkit->id);
		Item->SetPaintSeed(0);
		Item->SetPaintWear(0);
		Item->SetOrigin(8);
		Item->SetRarity(ITEM_RARITY_MYTHICAL);
		Item->SetLevel(1);
		Item->SetInUse(false);

		LocalInventory->AddEconItem(Item, 1, 0, 1);
#endif
	}

	void inventory_changer::change_page(int page, int weapon) {
		this->stage = page;

		switch (this->stage) {
		case 1:
		{
			items_weapons_paints->empty_children();


			if (items_weapons_list && items_weapons_list->children.size() == 0) {
				for (auto& weap : weapons)
					new inventory_item(items_weapons_list, weap, this->archive, std::bind(&inventory_changer::change_page, this, 2, weap->index_in_list));
			}


		}break;
		case 2:
		{
			items_weapons_paints->empty_children();
			items_weapons_paints->thumb_progress = 0.f;
			items_weapons_paints->scroll_progress = 0.f;
			auto weap = this->weapons.at(weapon);

			for (auto& paintkit : weapons.at(weapon)->paintkits)
				new inventory_item(items_weapons_paints, paintkit, this->archive, std::bind(&inventory_changer::painkit_modify, this, paintkit));

		}

		break;
		default:
			break;
		}
	}

	template<class T, class U>
	bool contains(const std::vector<T>& container, const U& v) {
		auto it = std::lower_bound(
			container.begin(),
			container.end(),
			v,
			[](const T& l, const U& r) { return l < r; });
		return it != container.end() && *it == v;
	}
	void  inventory_changer::painkit_modify(paintkit* pkit) {
		stage = 3;

		item_painkit_settings->empty_children();


		float child_rounding = 0.f;
		auto preview = new ui::child_window(GET_STR("1 "), 45.f, 50.0, ImColor(23, 24, 27, 255), item_painkit_settings, float_side::none, child_rounding, 15.f);

		{
			auto itm = new inventory_item(preview, pkit, this->archive, std::bind(&inventory_changer::change_page, this, 2, 0));
			itm->flags |= ui::flags::big_inventory_item;
		}

		auto settings = new ui::child_window(GET_STR("2"), 45.f, 50.0, ImColor(23, 24, 27, 255), item_painkit_settings, float_side::none, child_rounding, 15.f);

		{
			static float wear = 0.f;
			static float seed = 0;
			static float startrack = 0;


			new ui::slider(GET_STR("Wear"), settings, wear, 1.f, 25.f, ui::slider_type::floates);
			new ui::slider(GET_STR("Seed"), settings, seed, 1, 10, ui::slider_type::floates);
			auto inp = new ui::text_input(settings, GET_STR( "Default" ), this->search_text, text_type::string, 70.f, 50.f);
			//inp->title = "StatTrack";
			inp->flags |= ui::flags::text_input_as_element;

			auto inp2 = new ui::text_input(settings,  GET_STR("Name"), this->search_text, text_type::string, 70.f, 50.f);
		//	inp2->title = "Name";
			inp2->flags |= ui::flags::text_input_as_element;
		}

		auto buttons = new ui::child_window(GET_STR("3"), 45.f, 40.0, ImColor(255, 255, 255, 255), item_painkit_settings, float_side::none, child_rounding, 15.f); buttons->flex = flex_direction::block;

		//buttons->flags |= vertical_align_center; buttons->flags |= align_center;
		{
			auto free_side = new ui::child_window(GET_STR(""), 100.f, 60.0f, ImColor(255, 255, 255, 255), buttons, child_type::normal, flags::align_center | flags::vertical_align_center | flags::position_absolute);

			{

			}
			auto bottom_side = new ui::child_window(GET_STR(""), 100.f, 30.0f, ImColor(255, 255, 255, 255), buttons, child_type::normal, flags::align_center | flags::vertical_align_center | flags::position_absolute);

			{
				bottom_side->flex = flex_direction::automatic;
				bottom_side->padding = 0.f;


				auto left_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 24, 255, 255), bottom_side, child_type::normal, flags::align_left | flags::vertical_align_center | flags::position_absolute);

				{
					new ui::button(GET_STR("Back"), left_bottom_side, 80.f, 40.f, std::bind(&inventory_changer::change_page, this, 1, -1));

				}

				auto right_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 255, 255, 255), bottom_side, child_type::normal, flags::align_right | flags::vertical_align_center | flags::position_absolute);

				{

					new ui::button(GET_STR("Save"), right_bottom_side, 80.f, 40.f, std::bind(&inventory_changer::add_item_to_inventory, this, nullptr, pkit));

				}
			}
		}

	}
	inventory_changer::inventory_changer(object* parent) {

		on_start();


		this->parrent = parent;
		this->renderer = this->parrent->renderer;


		this->parrent->add_children(this);

		update();

		//	child_window ( std::string title_font, float percent_width, float percent_height, ImColor color, object * parent, float_side __float, float _rounding = 0.f, float _padding = 0.f ) {

		auto panel_container = new ui::panel_container(this, stage); panel_container->flags = flags::fullscreen;
		{

			auto active_weapons_list_panel = new ui::panel(panel_container); active_weapons_list_panel->flags = flags::fullscreen;
			active_weapons_list_panel->flex = flex_direction::automatic;
			{
				auto window = new ui::child_window(GET_STR(""), 100.f, 100.f, ImColor(255, 255, 255, 0), active_weapons_list_panel, child_type::normal, 0); window->flex = flex_direction::block; window->padding = 0;


				auto top_header = new ui::child_window(GET_STR("Weapons skins list"), 100.f, 10.f, ImColor(23, 24, 27, 255), window, float_side::none, 0.f, 0.f);

				top_header->flex = flex_direction::automatic;
				top_header->padding = 0.f;


				auto left_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 24, 255, 255), top_header, child_type::normal, flags::align_left | flags::vertical_align_center | flags::position_absolute);
				{
					//new ui::text_input ( left_bottom_side, "Search weapon", this->search_text, text_type::string, 70.f, 50.f );
				}

				auto right_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 255, 255, 255), top_header, child_type::normal, flags::align_right | flags::vertical_align_center | flags::position_absolute);
				{

					new ui::button(GET_STR("New weapon"), right_bottom_side, 40.f, 50.f, std::bind(&inventory_changer::change_page, this, 1, -1));

				}

				items_weapons_active = new ui::child_window(GET_STR("Items"), 100.f, 90.f, ImColor(23, 24, 27, 255), window, float_side::none, 0.f, 0.f); items_weapons_active->flags |= flags::hide_overflow;

				items_weapons_active->flags |= flags::scrollbar;

			}

			auto weapons_panel = new ui::panel(panel_container); weapons_panel->flags = flags::fullscreen;
			weapons_panel->flex = flex_direction::automatic;
			{
				auto window = new ui::child_window(GET_STR(""), 100.f, 100.f, ImColor(255, 255, 255, 0), weapons_panel, child_type::normal, flags::no_background); window->flex = flex_direction::block; window->padding = 0;

				auto top_header = new ui::child_window(GET_STR("Weapons csgo list"), 100.f, 10.f, ImColor(23, 24, 27, 255), window, float_side::none, 0.f, 0.f);

				top_header->flex = flex_direction::automatic;
				top_header->padding = 0.f;


				auto left_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 24, 255, 255), top_header, child_type::normal, flags::align_left | flags::vertical_align_center | flags::position_absolute);

				{
					new ui::text_input(left_bottom_side, GET_STR("Search weapon"), this->search_text, text_type::string, 70.f, 50.f);
				}

				auto right_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 255, 255, 255), top_header, child_type::normal, flags::align_right | flags::vertical_align_center | flags::position_absolute);

				{

					new ui::button(GET_STR("Back"), right_bottom_side, 40.f, 50.f, std::bind(&inventory_changer::change_page, this, 0, -1));

				}

				items_weapons_list = new ui::child_window(GET_STR("Items"), 100.f, 90.f, ImColor(23, 24, 27, 255), window, float_side::none, 0.f, 0.f); items_weapons_list->flags |= flags::hide_overflow;

				items_weapons_list->flags |= flags::scrollbar; items_weapons_list->flags |= flags::render_forward;

			}


			auto paint_kits_panel = new ui::panel(panel_container); paint_kits_panel->flags = flags::fullscreen;
			paint_kits_panel->flex = flex_direction::automatic;
			{
				auto window = new ui::child_window(GET_STR(""), 100.f, 100.f, ImColor(255, 255, 255, 0), paint_kits_panel, child_type::normal, flags::no_background); paint_kits_panel->flex = flex_direction::block; window->padding = 0;


				auto top_header = new ui::child_window(GET_STR("Paintkits list"), 100.f, 10.f, ImColor(23, 24, 27, 255), window, float_side::none, 0.f, 0.f);

				top_header->flex = flex_direction::automatic;
				top_header->padding = 0.f;


				auto left_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 24, 255, 255), top_header, child_type::normal, flags::align_left | flags::vertical_align_center | flags::position_absolute);

				{
					new ui::text_input(left_bottom_side, GET_STR("Search skin"), this->search_text, text_type::string, 70.f, 50.f);
				}

				auto right_bottom_side = new ui::child_window(GET_STR(""), 45.f, 100.0f, ImColor(255, 255, 255, 255), top_header, child_type::normal, flags::align_right | flags::vertical_align_center | flags::position_absolute);

				{

					new ui::button(GET_STR("Back"), right_bottom_side, 40.f, 50.f, std::bind(&inventory_changer::change_page, this, 1, -1));
				}

				items_weapons_paints = new ui::child_window(GET_STR("Items"), 100.f, 90.f, ImColor(23, 24, 27, 255), window, float_side::none, 0.f, 0.f); items_weapons_paints->flags |= flags::hide_overflow;

				items_weapons_paints->flags |= flags::scrollbar;
				items_weapons_paints->flags |= flags::render_forward;


			}

			auto panit_kit_preview_panel = new ui::panel(panel_container); panit_kit_preview_panel->flags = flags::fullscreen;
			panit_kit_preview_panel->flex = flex_direction::automatic;
			{


				item_painkit_settings = new ui::child_window(GET_STR(""), 100.f, 100.f, ImColor(255, 255, 255, 0), panit_kit_preview_panel, child_type::normal); item_painkit_settings->flex = flex_direction::automatic; item_painkit_settings->padding = 15;

			}



		}
	}

	/*	auto left_bottom_side = new ui::child_window ( "", 50.f, 100.0f, ImColor ( 255, 24, 255, 255 ), top_side, child_type::normal, flags::align_left | flags::vertical_align_center | flags::position_absolute );
		{

			new ui::text_input ( left_bottom_side, "Search weapon", this->search_text, text_type::string, 70.f, 50.f );



		}

		auto right_bottom_side = new ui::child_window ( "", 50.f, 100.0f, ImColor ( 255, 255, 255, 255 ), top_side, child_type::normal, flags::align_right | flags::vertical_align_center | flags::position_absolute );
		{
			new ui::button ( "New skin", right_bottom_side, 40.f, 50.f, refresh_ext );
		}
*/
//	auto center_side = new child_window ( "sadds", 100.f, 50.f, ImColor ( 255, 255, 255, 0 ), weapons_panel, float_side::none, 0.f, 0.f );
	//center_side->flags |= flags::hide_overflow;

	//for(auto& weap : weapons )
		//new inventory_item ( center_side, weap );



	void inventory_changer::parse_data() {
		std::ifstream in("skins.json");

		nlohmann::json json_data; in >> json_data;


	}

	void copy_inv_item(inventory_item* A, inventory_item* B) {




	}
	bool contains(const std::string& word, const std::string& sentence) {
		if (word == "" || sentence == "")
			return true;

		return sentence.find(word) != std::string::npos;
	}

	std::string to_lower(std::string str) {
		std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::tolower);

		return str;
	}
	bool search(char* pat, char* txt) {
		int M = strlen(pat);
		int N = strlen(txt);

		/* A loop to slide pat[] one by one */
		for (int i = 0; i <= N - M; i++) {
			int j;

			/* For current index i, check for pattern match */
			for (j = 0; j < M; j++)
				if (txt[i + j] != pat[j])
					break;

			if (j == M) // if pat[0...M-1] = txt[i, i+1, ...i+M-1]
				return true;
		}

		return false;
	}
	void inventory_changer::draw() {
		handle();

		this->renderer->AddRectFilled(this->mins, this->maxs, ImColor(255, 0, 0, 255));



		this->handle();







		if (this->search_text.size() != last_search_size) {


			if (this->search_text.size() == 0 && last_search_size != 0) {
				items_weapons_list->empty_children();

				items_weapons_list->children.reserve(60);

				for (auto& child : this->original_children) {
					auto wpn = new ui::inventory_item(static_cast<ui::inventory_item*>(child));
					items_weapons_list->add_children(wpn);

					update();

				}
				last_search_size = 0;
			}
			else {


				if (this->search_text.size() > 0 && last_search_size == 0) {


					for (auto& c : this->original_children)
						delete c;

					this->original_children.clear();

					this->original_children.reserve(60);

					for (auto& child : items_weapons_list->children) {
						this->original_children.push_back(new ui::inventory_item(static_cast<ui::inventory_item*>(child)));

					}

				}
				else if (!this->search_text.empty()) {
					items_weapons_list->empty_children();

					items_weapons_list->children.reserve(60);

					for (auto& child : this->original_children) {
						auto itm = static_cast<inventory_item*>(child);

						if (search(this->search_text.data(), itm->weap->display_name.data())) {
							auto wpn = new ui::inventory_item(static_cast<ui::inventory_item*>(child));
							items_weapons_list->add_children(wpn);

							update();
						}
					}
				}
			}

			last_search_size = this->search_text.size();
		}

		for (auto& child : this->children) {
			child->draw();

		}
	}
	void inventory_changer::handle_mouse_input() {
		if (ui::focused_item != -1)
			return;

		auto mouse_pos = ui::get_cursor();

		this->hovering = (mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x&& mouse_pos.y < this->maxs.y);
	}

	void inventory_changer::handle() {

		handle_mouse_input();
		auto mouse_pos = ui::get_cursor();




	}


	void inventory_changer::update() {

		static int last_child_size = 0;

		this->mins.x = this->parrent->mins.x + 30;
		this->maxs.x = this->parrent->maxs.x - 30;

		this->mins.y = this->parrent->mins.y + 10;
		this->maxs.y = this->parrent->maxs.y - 50;

		this->width = this->maxs.x - this->mins.x;
		this->height = this->maxs.y - this->mins.y;


		for (auto& child : children) {
			child->update();
		}

	}

	weapon::weapon(int item_definition_index) {


	}

	void paintkit::generate_image_path() {

		auto skin_name = game_name;


		if (strcmp(weapon_name.c_str(), crypt_str("unknown")) && strcmp(weapon_name.c_str(), crypt_str("knife")) && strcmp(weapon_name.c_str(), crypt_str("gloves"))) //-V526
		{
			if (skin_name.empty() || skin_name == crypt_str("default"))
				image_path = crypt_str("resource/flash/econ/weapons/base_weapons/") + std::string(weapon_name) + crypt_str(".png");
			else
				image_path = crypt_str("resource/flash/econ/default_generated/") + std::string(weapon_name) + crypt_str("_") + std::string(skin_name) + crypt_str("_light_large.png");
		}
		else {
			if (!strcmp(weapon_name.c_str(), crypt_str("knife")))
				image_path = crypt_str("resource/flash/econ/weapons/base_weapons/weapon_knife.png");
			else if (!strcmp(weapon_name.c_str(), crypt_str("gloves")))
				image_path = crypt_str("resource/flash/econ/weapons/base_weapons/ct_gloves.png");
			else if (!strcmp(weapon_name.c_str(), crypt_str("unknown")))
				image_path = crypt_str("resource/flash/econ/weapons/base_weapons/weapon_snowball.png");

		}
	}
}