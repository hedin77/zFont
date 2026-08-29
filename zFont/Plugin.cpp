// This file added in headers queue
// File: "Sources.h"

namespace GOTHIC_ENGINE {
  void Game_Entry() {
    HMODULE handle = GetModuleHandle( "ddraw.dll" );
    char buffer[1024];
    GetModuleFileName( handle, buffer, 1023 );
    string ddrawFileName = buffer;
    CustomDirectDraw = ddrawFileName.StartWith( Union.GetGameDirectory() );

    string temp;
    Union.GetSysPackOption().Read( FontScale, "Font", "Scale", FontScale );
    Union.GetSysPackOption().Read( DrawShadow, "Font", "DrawShadow", DrawShadow );
    Union.GetSysPackOption().Read( DrawHighlight, "Font", "DrawHighlight", DrawHighlight );
    Union.GetSysPackOption().Read( DefaultSystemFont, "Font", "DefaultSystemFont", DefaultSystemFont );
    Union.GetSysPackOption().Read( StaticEncoding, "Font", "StaticEncoding", StaticEncoding );
    Union.GetSysPackOption().Read(temp, "Font", "FONT_DEFAULT.TGA", "1.0, 205, 186, 156");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_OLD_10_WHITE.TGA", "1.0, 205, 186, 156");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_OLD_10_WHITE_HI.TGA", "1.0, 255, 255, 255");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_OLD_20_WHITE.TGA", "1.0, 255, 222, 172");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_OLD_20_WHITE_HI.TGA", "1.0, 255, 255, 255");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_10_BOOK.TGA", "1.0, 15, 15, 15");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_10_BOOK_HI.TGA", "1.0, 30, 30, 30");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_15_BOOK.TGA", "1.0, 15, 15, 15");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_15_BOOK_HI.TGA", "1.0, 30, 30, 30");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_20_BOOK.TGA", "1.0, 77, 77, 255");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_20_BOOK_HI.TGA", "1.0, 51, 204, 204");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_OLD_10_WHITE_HO.TGA", "1.0, 255, 255, 255");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_OLD_20_MENU.TGA", "1.0, 205, 186, 156");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_OLD_20_MENU_HI.TGA", "1.0, 255, 255, 255");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_SYMDICE.TGA", "1.0, 255, 100, 0");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_SYMBOLS.TGA", "1.0, 204, 242, 255");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_POISON.TGA", "1.0, 255, 102, 0");
    Union.GetSysPackOption().Read(temp, "Font", "FONT_STAMINA.TGA", "1.0, 255, 0, 0");


    int count = Union.GetSysPackOption().lstBlocks.GetNumInList();
    for (int i = count - 1; i >= 0; i--) {
        COption::OptBlock* block = Union.GetSysPackOption().lstBlocks.GetAt(i)->GetData(); 
        if (block && block->sBlockName.CompareI("FONT")) {
            int sizeValues = block->lstValues.GetNumInList();
            for (int j = sizeValues - 1; j >= 0; j--) {
                COption::OptBlock::OptValue* optValue = block->lstValues.GetAt(j)->GetData();
                if (optValue && optValue->sValueName.StartWith("FONT_")) {
                    Array<CStringA> params = optValue->sValue.Split(",");
                    FontsCustomMultiplier.Insert(A optValue->sValueName, params.GetSafe(0)->ToReal32());
                    FontsColors.Insert(A optValue->sValueName, optValue->sValue);
                }
            }
        }
    }

    if( StaticEncoding != 65001 && StaticEncoding < 1250 && StaticEncoding > 1258 )
      StaticEncoding = 0;
  }

  void Game_Exit() {
  }

  void Game_Init() {
    bool needPartialHook = true;
    Union.GetSysPackOption().Read(needPartialHook, "Font", "NeedPartialHook", needPartialHook);
    if (needPartialHook) {
        initHook();
    }
    // DefineCurrentLanguage();
    GameEncoding = GuessGameEncoding();
    cmd << "Choosen CP: " << GameEncoding << endl;
  }

  void Game_PreLoop() {
  }

  void Game_Loop() {
    if( StaticEncoding == 0 && !ogame->IsOnPause() ) {
      SwapGuessedEncodings();
    }
  }

  void Game_PostLoop() {
  }

  void Game_MenuLoop() {
    if( StaticEncoding == 0 ) {
      SwapGuessedEncodings();
    }
  } 

  // Information about current saving or loading world
  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

  void Game_SaveBegin() {
  }

  void Game_SaveEnd() {
  }

  void LoadBegin() {
  }

  void LoadEnd() {
  }

  void Game_LoadBegin_NewGame() {
    LoadBegin();
  }

  void Game_LoadEnd_NewGame() {
    LoadEnd();
  }

  void Game_LoadBegin_SaveGame() {
    LoadBegin();
  }

  void Game_LoadEnd_SaveGame() {
    LoadEnd();
  }

  void Game_LoadBegin_ChangeLevel() {
    LoadBegin();
  }

  void Game_LoadEnd_ChangeLevel() {
    LoadEnd();
  }

  void Game_LoadBegin_Trigger() {
  }
  
  void Game_LoadEnd_Trigger() {
  }
  
  void Game_Pause() {
  }
  
  void Game_Unpause() {
  }
  
  void Game_DefineExternals() {
  }

  void Game_ApplyOptions() {
  }

  /*
  Functions call order on Game initialization:
    - Game_Entry           * Gothic entry point
    - Game_DefineExternals * Define external script functions
    - Game_Init            * After DAT files init
  
  Functions call order on Change level:
    - Game_LoadBegin_Trigger     * Entry in trigger
    - Game_LoadEnd_Trigger       *
    - Game_Loop                  * Frame call window
    - Game_LoadBegin_ChangeLevel * Load begin
    - Game_SaveBegin             * Save previous level information
    - Game_SaveEnd               *
    - Game_LoadEnd_ChangeLevel   *
  
  Functions call order on Save game:
    - Game_Pause     * Open menu
    - Game_Unpause   * Click on save
    - Game_Loop      * Frame call window
    - Game_SaveBegin * Save begin
    - Game_SaveEnd   *
  
  Functions call order on Load game:
    - Game_Pause              * Open menu
    - Game_Unpause            * Click on load
    - Game_LoadBegin_SaveGame * Load begin
    - Game_LoadEnd_SaveGame   *
  */

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_PreLoop,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_PostLoop,
    Enabled( AppDefault ) Game_MenuLoop,
    Enabled( AppDefault ) Game_SaveBegin,
    Enabled( AppDefault ) Game_SaveEnd,
    Enabled( AppDefault ) Game_LoadBegin_NewGame,
    Enabled( AppDefault ) Game_LoadEnd_NewGame,
    Enabled( AppDefault ) Game_LoadBegin_SaveGame,
    Enabled( AppDefault ) Game_LoadEnd_SaveGame,
    Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
    Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
    Enabled( AppDefault ) Game_LoadBegin_Trigger,
    Enabled( AppDefault ) Game_LoadEnd_Trigger,
    Enabled( AppDefault ) Game_Pause,
    Enabled( AppDefault ) Game_Unpause,
    Enabled( AppDefault ) Game_DefineExternals,
    Enabled( AppDefault ) Game_ApplyOptions
  );
}