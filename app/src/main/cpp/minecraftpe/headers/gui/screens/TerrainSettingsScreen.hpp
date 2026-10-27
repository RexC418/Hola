#pragma once
#include <gui/Screen.hpp>
#include <gui/buttons/Touch_TButton.hpp>
#include <gui/buttons/Touch_THeader.hpp>
#include <gui/buttons/ImageButton.hpp>
#include <gui/NinePatchLayer.hpp>
#include <network/mco/MCOServerListItem.hpp>

struct Minecraft;

/*
 * Arrow button used by the "Terrain Generation" selector.
 * Same shape as the reference image (a square button with a triangular arrow
 * pointing left/right next to the long center button), but built from the
 * original MCPE 0.8.1 button textures (nine-patch from gui/spritesheet.png)
 * plus a custom pixel-art arrow texture created to match the reference shape
 * while keeping the MCPE 0.8.1 pixelated aesthetic and original colors.
 */
struct TerrainArrowButton : ImageButton {
	NinePatchLayer* normal;
	NinePatchLayer* pressed;

	TerrainArrowButton(int32_t id, const struct ImageDef& def, NinePatchLayer* normalLayer, NinePatchLayer* pressedLayer);
	virtual ~TerrainArrowButton() {
	}
	virtual void renderBg(Minecraft*, int32_t, int32_t);
	virtual bool_t isSecondImage(bool_t) {
		return 0;
	}
};

/*
 * "Terrain Settings" screen, opened from the "Terrain Settings" button on the
 * Create World screen. Shows the "Terrain Generation" selector:
 *
 *     [ < ]  [ === RD-132211 === ]  [ > ]
 *
 * - The long center button (the red part in the reference image) always shows
 *   the name of the version the selected terrain generator belongs to.
 * - The two green-side buttons from the reference image go back / advance
 *   through the generator list. The cycling logic is fully implemented, but
 *   while RD-132211 is the only generator in the list they do nothing at all.
 * - The selection is applied to the world that is being created.
 */
struct TerrainSettingsScreen : Screen {
	Touch::THeader* header;
	Touch::TButton* backButton;
	Touch::TButton* generatorButton;
	TerrainArrowButton* prevButton;
	TerrainArrowButton* nextButton;
	NinePatchLayer* arrowNormal;
	NinePatchLayer* arrowPressed;
	MCOServerListItem field_170;

	TerrainSettingsScreen(const MCOServerListItem&);
	void closeScreen();
	void refreshGeneratorButton();

	virtual ~TerrainSettingsScreen();
	virtual void render(int32_t, int32_t, float);
	virtual void init();
	virtual void setupPositions();
	virtual bool_t handleBackEvent(bool_t);
	virtual void buttonClicked(Button*);
};
