/*
  Copyright (C) 2012-2013 - Voidious

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BBENGINE_H
#define BBENGINE_H

#include <exception>
#include <pthread.h>
#include "bbconst.h"
#include "bbutil.h"
#include "stage.h"
#include "filemanager.h"
#include "sensorhandler.h"
#include "replaybuilder.h"
#include "printhandler.h"

#define PCALL_STAGE     1
#define PCALL_SHIP      2
#define PCALL_VALIDATE  3

extern "C" {
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}

class EngineException : public std::exception {
  char *message_;
  
  public:
    EngineException(const char *details);
    EngineException(const char *filename, const char *details);
    ~EngineException() throw();
    virtual const char* what() const throw();
};

typedef struct {
  lua_State *L;
  volatile unsigned long timerTick;
  volatile unsigned long timerExpiration;
  volatile bool enabled;
} TickTimerSettings;

class BerryBotsEngine {
  Stage *stage_;
  PrintHandler *printHandler_;
  FileManager *fileManager_;
  lua_State *stageState_;
  char *stagesDir_;
  char *stageFilename_;
  World *stageWorld_;

  Team **teams_;
  Ship **ships_;
  // TODO: Rename stageShips to something else to avoid confusion with ships
  //       loaded by the stage 'configure' function.
  Ship **stageShips_;  // the copy of all ships owned by the stage program
  Ship **oldShips_;    // the ships at the start of the current tick
  Ship **prevShips_;   // the ships at the start of the previous tick
  ShipProperties **shipProperties_;
  int numTeams_;
  int numInitializedTeams_;
  int teamSize_;
  int numShips_;
  int numInitializedShips_;
  World **worlds_;
  ShipGfx **shipGfxs_;
  StageGfx *stageGfx_;
  bool** teamVision_;
  pthread_t timerThread_;
  TickTimerSettings *timerSettings_;

  int gameTime_;
  SensorHandler *sensorHandler_;
  bool stageRun_;
  bool stageConfigureComplete_;
  bool shipInitComplete_;
  bool battleMode_;
  bool roundOver_;
  bool gameOver_;
  bool physicsOver_;
  char winnerName_[MAX_NAME_LENGTH + 1];
  char winnerFilename_[MAX_NAME_LENGTH + 1];
  bool hasRanks_;
  bool hasScores_;
  ReplayBuilder *replayBuilder_;
  ReplayEventHandler *replayHandler_;
  char *replayTemplateDir_;

  public:
    BerryBotsEngine(PrintHandler *printHandler, FileManager *manager,
                    const char *replayTemplateDir);
    ~BerryBotsEngine();

    bool isStageConfigureComplete();
    bool isShipInitComplete();
    void setBattleMode(bool battleMode);
    bool isBattleMode();
    void nextRound();
    void setRoundOver(bool roundOver);
    bool isRoundOver();
    void setGameOver(bool gameOver);
    bool isGameOver();
    void setWinnerName(const char *winnerName);
    void setRank(const char *teamName, int rank);
    void setScore(const char *teamName, double score);
    void setStatistic(const char *teamName, const char *key, double value);
    const char* getWinnerName();
    const char* getWinnerFilename();
    TeamResult** getTeamResults();
    bool hasScores();

    void initStage(const char *stagesBaseDir, const char *stageName,
                   const char *cacheDir) throw (EngineException*);
    void initShips(const char *shipsBaseDir, char **teamNames, int numTeams,
                   const char *cacheDir) throw (EngineException*);
    void stagePrint(const char *text);
    void shipPrint(lua_State *L, const char *text);
    void processTick() throw (EngineException*);
    void processRoundOver();
    void processGameOver();
    void monitorCpuTimer(Team *team, bool fatal);

    Stage* getStage();
    Team** getTeams();
    Team* getTeam(int teamIndex);
    Team* getTeam(lua_State *L);
    Team** getRankedTeams();
    int getNumTeams();
    Ship** getShips();
    int getNumShips();
    Ship* getStageProgramShip(const char *name);
    int getGameTime();
    void setTeamSize(int teamSize);
    int getTeamSize();

    bool touchedZone(Ship *ship, const char *zoneTag);
    bool touchedAnyZone(Ship *ship);
    void destroyShip(Ship *ship);
    static void* timer(void *vargs);
    int callUserLuaCode(lua_State *L,int nargs, const char *errorMsg,
                        int callStyle) throw (EngineException*);
    ReplayBuilder* getReplayBuilder();
  private:
    void initReplayBuilder(int numTeams, int numShips, Stage *stage);
    void processWinnerRanksScores();
    void setTeamRanksByScore();
    void initShipRound(Ship *ship);
    void updateTeamShipsAlive();
    void processStageRun() throw (EngineException*);
    void uniqueShipNames(Ship** ships, int numShips);
    void uniqueTeamNames(Team** teams, int numTeams);
    void copyShips(Ship **srcShips, Ship **destShips, int numShips);
    void printLuaErrorToShipConsole(lua_State *L, const char *formatString);
    void throwForLuaError(lua_State *L, const char *formatString)
        throw (EngineException*);
    char* formatLuaError(lua_State *L, const char *formatString);
};

#endif
