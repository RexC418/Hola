#include <gui/screens/TerrainSettingsScreen.hpp>
#include <gui/screens/CreateWorldScreen.hpp>
#include <Minecraft.hpp>
#include <gui/NinePatchFactory.hpp>
#include <util/IntRectangle.hpp>
#include <util/ImageDef.hpp>
#include <util/TerrainGen.hpp>
#include <rendering/Tesselator.hpp>
#include <rendering/Textures.hpp>
#include <cpputils.hpp>

TerrainArrowButton::TerrainArrowButton(int32_t id, const struct ImageDef& def, NinePatchLayer* normalLayer, NinePatchLayer* pressedLayer)
                : ImageButton(id, "", def) {
        this->normal = normalLayer;
        this->pressed = pressedLayer;
}

void TerrainArrowButton::renderBg(struct Minecraft* mc, int32_t x, int32_t y) {
        this->normal->setSize((float)this->width, (float)this->height);
        this->pressed->setSize((float)this->width, (float)this->height);

        NinePatchLayer* layer;
        if(this->active && this->pressed && x >= this->posX && y >= this->posY && x < (this->posX + this->width) && y < (this->posY + this->height)) {
                layer = this->pressed;
        } else {
                layer = this->normal;
        }
        layer->draw(Tesselator::instance, (float)this->posX, (float)this->posY);
}

TerrainSettingsScreen::TerrainSettingsScreen(const MCOServerListItem& a2) {
        this->header = 0;
        this->backButton = 0;
        this->generatorButton = 0;
        this->prevButton = 0;
        this->nextButton = 0;
        this->arrowNormal = 0;
        this->arrowPressed = 0;
        this->field_170 = a2;
}

TerrainSettingsScreen::~TerrainSettingsScreen() {
        safeRemove<Touch::TButton>(this->backButton);
        safeRemove<Touch::TButton>(this->generatorButton);
        safeRemove<TerrainArrowButton>(this->prevButton);
        safeRemove<TerrainArrowButton>(this->nextButton);
        safeRemove<Touch::THeader>(this->header);
        safeRemove<NinePatchLayer>(this->arrowNormal);
        safeRemove<NinePatchLayer>(this->arrowPressed);
}

void TerrainSettingsScreen::closeScreen() {
        // Return to the Create World screen (a fresh instance, like the vanilla
        // MCPE 0.8.1 screen flow does), carrying over the draft world settings.
        this->minecraft->setScreen(new CreateWorldScreen(WST_LOCALGAME, this->field_170));
}

void TerrainSettingsScreen::refreshGeneratorButton() {
        this->generatorButton->setMsg(TerrainGen::getSelectedName());
}

void TerrainSettingsScreen::init() {
        this->header = new Touch::THeader(0, "Terrain Settings");
        this->backButton = new Touch::TButton(1, "Back", 0);
        this->backButton->width = 38;
        this->backButton->height = 18;
        this->backButton->init(this->minecraft);

        // Center part of the "Terrain Generation" selector: shows the name of the
        // version the selected terrain generator comes from (RD-132211 for now).
        this->generatorButton = new Touch::TButton(2, "", 0);
        this->generatorButton->init(this->minecraft);
        this->refreshGeneratorButton();

        // Nine-patch button textures for the two arrow buttons (original MCPE 0.8.1
        // button look: same sprites as the "Back" / "Generate World" buttons).
        NinePatchFactory npf(this->minecraft->texturesPtr, "gui/spritesheet.png");
        this->arrowNormal = npf.createSymmetrical(IntRectangle(8, 32, 8, 8), 2, 2, 24, 24);
        this->arrowPressed = npf.createSymmetrical(IntRectangle(0, 32, 8, 8), 2, 2, 24, 24);

        // Custom pixel-art arrows (24x24 each, packed side by side in one texture)
        // shaped like the reference image but keeping the original MCPE style.
        ImageDef leftArrow;
        leftArrow.field_0 = "gui/terrain_arrows.png";
        leftArrow.width = 24;
        leftArrow.height = 24;
        leftArrow.field_14 = 0;
        leftArrow.field_18 = 0;
        leftArrow.field_1C = 24;
        leftArrow.field_20 = 24;
        leftArrow.field_24 = 1;

        ImageDef rightArrow;
        rightArrow.field_0 = "gui/terrain_arrows.png";
        rightArrow.width = 24;
        rightArrow.height = 24;
        rightArrow.field_14 = 24;
        rightArrow.field_18 = 0;
        rightArrow.field_1C = 24;
        rightArrow.field_20 = 24;
        rightArrow.field_24 = 1;

        this->prevButton = new TerrainArrowButton(3, leftArrow, this->arrowNormal, this->arrowPressed);
        this->prevButton->width = 24;
        this->prevButton->height = 24;
        this->nextButton = new TerrainArrowButton(4, rightArrow, this->arrowNormal, this->arrowPressed);
        this->nextButton->width = 24;
        this->nextButton->height = 24;

        this->buttons.push_back(this->header);
        this->buttons.push_back(this->backButton);
        this->buttons.push_back(this->generatorButton);
        this->buttons.push_back(this->prevButton);
        this->buttons.push_back(this->nextButton);
}

void TerrainSettingsScreen::setupPositions() {
        this->backButton->posX = 4;
        this->backButton->posY = 4;
        this->header->posX = 0;
        this->header->posY = 0;
        this->header->width = this->width;
        this->header->height = this->backButton->height + 8;

        // Selector widget centered on screen, like in the reference image:
        // [ < ] [ === center === ] [ > ]
        int32_t selectorHeight = 26;
        int32_t centerY = this->height / 2 - selectorHeight / 2;
        int32_t arrowSize = 24;
        int32_t gap = 3;
        int32_t selectorWidth = this->width < 260 ? this->width - 24 : 236;
        int32_t startX = this->width / 2 - selectorWidth / 2;

        this->prevButton->width = arrowSize;
        this->prevButton->height = arrowSize;
        this->prevButton->posX = startX;
        this->prevButton->posY = centerY + (selectorHeight - arrowSize) / 2;

        this->nextButton->width = arrowSize;
        this->nextButton->height = arrowSize;
        this->nextButton->posX = startX + selectorWidth - arrowSize;
        this->nextButton->posY = centerY + (selectorHeight - arrowSize) / 2;

        this->generatorButton->width = selectorWidth - arrowSize * 2 - gap * 2;
        this->generatorButton->height = selectorHeight;
        this->generatorButton->posX = startX + arrowSize + gap;
        this->generatorButton->posY = centerY;
}

void TerrainSettingsScreen::render(int32_t x, int32_t y, float a4) {
        this->renderMenuBackground(a4);

        // "Terrain Generation" title right above the selector widget.
        this->drawCenteredString(this->font, "Terrain Generation", this->width / 2, this->height / 2 - 34, 0xE0E0E0);

        Screen::render(x, y, a4);
}

bool_t TerrainSettingsScreen::handleBackEvent(bool_t a2) {
        if(!a2) {
                this->closeScreen();
        }
        return 1;
}

void TerrainSettingsScreen::buttonClicked(Button* a2) {
        if(a2 == this->backButton) {
                this->closeScreen();
                return;
        }
        if(a2 == this->prevButton) {
                // Left green arrow: go back in the generator selection.
                TerrainGen::cycle(-1);
                this->refreshGeneratorButton();
                return;
        }
        if(a2 == this->nextButton) {
                // Right green arrow: advance in the generator selection.
                TerrainGen::cycle(1);
                this->refreshGeneratorButton();
                return;
        }
        // The long center button only displays the currently selected generator
        // version name, exactly like the reference image.
}
