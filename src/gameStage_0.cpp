#include "gameStage_0.h"
#include "math.h"
#include <math.h>


void Stage0::render() {
	// FrameBuffer
	Image fb(160, 120);

	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	//do not change framebuffer size
	int cs = 8;
	int numtiles = 18;
	Vector2 player_print_coords = { fb.width / 2 - player.size.x / 2 + cb.player_cam.x , fb.height / 2 - player.size.y / 2 + cb.player_cam.y };

	fb.fill(bgcolor);	//fills the image with one color

	// Size in pixels of a cell

	GameMap::drawTileset(map, cs, numtiles, tileset, fb, cb.cam_offset, time);

	GameMap::drawAsset(fb, tutorial_help1, Vector2(3 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);
	GameMap::drawAsset(fb, keyset, Area(17 * (int(time * 1.5) % 2), 34, 17, 17), Vector2(10 * 8 + 4, 21 * 8 - 3), cb.cam_offset, player.coords);
	GameMap::drawAsset(fb, keyset, Area(17 * (int(time * 1.5 + 0.33) % 2), 4 * 17, 17, 17), Vector2(8 * 8 + 2, 19 * 8 - 5), cb.cam_offset, player.coords);
	GameMap::drawAsset(fb, keyset, Area(17 * (int(time * 1.5 + 0.69) % 2), 3 * 17, 17, 17), Vector2(6 * 8, 21 * 8 - 3), cb.cam_offset, player.coords);
	GameMap::drawAsset(fb, keyset, Area(0, 5 * 17, 17, 17), Vector2(8 * 8 + 2, 21 * 8 - 3), cb.cam_offset, player.coords);


	GameMap::drawAsset(fb, tutorial_help2, Vector2(40 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);
	if (time - tuto2_timer < 0.66) {
		GameMap::drawAsset(fb, keyset, Area(17 * (int((time - tuto2_timer) * 6) % 2), 34, 17, 17), Vector2(47 * 8 + 8, 21 * 8 - 3), cb.cam_offset, player.coords);
	}
	else if ((time - tuto2_timer < 1.5)) {
		GameMap::drawAsset(fb, keyset, Area(0, 34, 17, 17), Vector2(47 * 8 + 8, 21 * 8 - 3), cb.cam_offset, player.coords);
	}
	else {
		GameMap::drawAsset(fb, keyset, Area(0, 34, 17, 17), Vector2(47 * 8 + 8, 21 * 8 - 3), cb.cam_offset, player.coords);
		tuto2_timer = time;
	}

	GameMap::drawAsset(fb, keyset, Area(0, 4 * 17, 17, 17), Vector2(45 * 8 + 6, 19 * 8 - 5), cb.cam_offset, player.coords);
	GameMap::drawAsset(fb, keyset, Area(0, 3 * 17, 17, 17), Vector2(43 * 8 + 4, 21 * 8 - 3), cb.cam_offset, player.coords);
	GameMap::drawAsset(fb, keyset, Area(0, 5 * 17, 17, 17), Vector2(45 * 8 + 6, 21 * 8 - 3), cb.cam_offset, player.coords);

	GameMap::drawAsset(fb, tutorial_help3, Vector2(80 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);

	GameMap::drawAsset(fb, tutorial_help4, Vector2(112 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);
	GameMap::drawAsset(fb, keyset, Area(17 * (int(time * 2) % 2), 0, 17, 17), Vector2(119 * 8, 16 * 8), cb.cam_offset, player.coords);



	fb.drawText("FPS: " + toString(int(fps)), 125, 1, minifont, 4, 6);

	if (!player.teleporting) {
		fb.drawImage(sprite_mini,
			player_print_coords.x,
			player_print_coords.y,
			Area(player.moving * player.size.x * (int(time * 8) % 6),
				player.direction * player.size.y,
				player.size.x,
				player.size.y));	//draws only a part of an image
		if (time - player.last_ground < 0.5) {
			fb.drawImage(grounding,
				player.last_ground_pos.x - 12 + cb.cam_offset.x,
				player.last_ground_pos.y - 12 + cb.cam_offset.y,
				Area(32 * ((int((time - player.last_ground) * 8) % 4)),
					0,
					32,
					32));	//draws only a part of an image
		}
	}
	else if (time - player.last_death < 0.87) {
		fb.drawImage(explosion,
			player_print_coords.x - 12,
			player_print_coords.y - 12,
			Area(32 * ((int((time - player.last_death) * 8) % 8)),
				7 * 32,
				32,
				32));	//draws only a part of an image
	}
	else {
		Vector2 what = Vector2(player.velocity.x, player.velocity.y).normalize_unit(400) / 30;
		std::cout << what.x << " " << what.y << "\n";
		fb.drawLine(player_print_coords.x + 4,
			player_print_coords.y + 4,
			player_print_coords.x - what.x + 4,
			player_print_coords.y - what.y + 4,
			Color::RANDOM());
	}
	fb.drawImage(clock_sprite, 148, 71, Area(9 * (int(time * 25) % 16), 0, 9, 11));

	fb.drawRectangle(150, 84, 5, 31, Color::GRAY);
	fb.drawRectangle(151, 85, 3, 30 * (idx_should - idx_lowest) / 1000, Color::CYAN);
	fb.drawRectangle(143, 110, 5, 5, Color::GRAY);
	fb.drawRectangle(144, 111, 3, 6 * Game::norm(0.5, time - last_fired), Color::RED);

	// Draw region of tileset inside framebuffer
	// fb.drawImage(tileset, screenx, screeny, area);
	for (sEntity bullet : bullets) {
		int screenx = bullet.coords.x + cb.cam_offset.x;
		int screeny = bullet.coords.y + cb.cam_offset.y;
		fb.drawLine(screenx, screeny, screenx + 3, screeny, Color::RED);
	}


	//fb.drawText("BoxBreak: " + toString(box_breakable.active) + " | KeyThing: " + toString(key_tobreak.active), 2, 2, minifont, 4, 6);
	//fb.drawText("TR: " + toString(box_breakable.hitbox.t_r.x) + ":" + toString(box_breakable.hitbox.t_r.y) +
	//	" | BL: " + toString(box_breakable.hitbox.b_l.x) + ":" + toString(box_breakable.hitbox.b_l.y), 2, 9, minifont, 4, 6);

	//if (box_breakable.active) {
	//	fb.drawImage(simple_box, box_breakable.coords.x + cb.cam_offset.x, box_breakable.coords.y + cb.cam_offset.y - 40, Area(0, 0, 13, 40));
	//	fb.drawImage(opening_keys, box_breakable.coords.x + cb.cam_offset.x + 3, box_breakable.coords.y + cb.cam_offset.y - 22 + int(time * 2) % 2, Area(0, 0, 7, 4));
	//}
	//else if (time - box_break_time < 0.8) {
	//	fb.drawImage(simple_box, box_breakable.coords.x + cb.cam_offset.x, box_breakable.coords.y + cb.cam_offset.y - 40, Area(13 * (int(8 * (time - box_break_time)) % 8), 0, 13, 40));
	//}
	//else {
	//	fb.drawImage(simple_box, box_breakable.coords.x + cb.cam_offset.x, box_breakable.coords.y + cb.cam_offset.y - 40, Area(13 * 7, 0, 13, 40));
	//}
	//if (key_tobreak.active) {
	//	fb.drawImage(opening_keys, key_tobreak.coords.x + cb.cam_offset.x, key_tobreak.coords.y + cb.cam_offset.y + int(time * 2) % 2, Area(0, 0, 7, 4));
	//}


	// DRAW HITBOXES
	//float player_print_hitbox[4] = { player_print_coords.x + player.size.x / 3, player_print_coords.y + player.size.y,
	//							player_print_coords.x + player.size.x * 2 / 3, player_print_coords.y + player.size.y * 5 / 6 };
	//fb.drawLine(player_print_hitbox[0], player_print_hitbox[1] - 1, player_print_hitbox[2], player_print_hitbox[3] - 1, Color::RED);
	//fb.drawLine(player_print_hitbox[2], player_print_hitbox[1] - 1, player_print_hitbox[0], player_print_hitbox[3] - 1, Color::RED);


	if (reverting) {
		fb.drawImage(revert_overlay, 0, 0);
		fb.drawText(toString((float)(idx_should - idx_lowest) / fps), 30, 51, bigfont, 14, 18);
	}
}

