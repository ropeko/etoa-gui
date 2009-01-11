
#ifndef __ENTITY__
#define __ENTITY__

#include <string>
#include <mysql++/mysql++.h>

#include "../MysqlHandler.h"
#include "../functions/Functions.h"
#include "../data/DataHandler.h"
#include "../objects/User.h"
#include "../objects/Object.h"
#include "../objects/Fleet.h"

/**
* Entity class
* 
* @author Stephan Vock<glaubinx@etoa.ch>
*/

class Entity	
{
	public: 
		Entity(char code, mysqlpp::Row &eRow) {
			if (eRow) {
				this->id = (int)eRow["id"];
				this->cellId = (int)eRow["cell_id"];
				this->code = code;
				this->pos = (short)eRow["pos"];
				this->lastVisited = (int)eRow["lastvisited"];
			} else {
				this->id = 0;
				this->cellId = 0;
				this->code = code;
				this->pos = 0;
				this->lastVisited = 0;
			}
			
			this->resMetal = 0;
			this->resCrystal = 0;
			this->resPlastic = 0;
			this->resFuel = 0;
			this->resFood = 0;
			this->resPower = 0;
			this->resPeople = 0;
			
			this->wfMetal = 0;
			this->wfCrystal = 0;
			this->wfPlastic = 0;
			
			this->userMain = false;
			this->typeId = 0;
			this->userChanged = 0;
			
			this->initWeapon = -1;
			this->initShield = -1;
			this->initStructure = -1;
			this->initStructShield = -1;
			this->initHeal = -1;
			this->initCount = -1;
			
			this->weapon = 0;
			this->shield = 0;
			this->structure = 0;
			this->heal = 0;
			this->count = 0;
			this->defCount = 0;
			this->healCount = 0;
			this->spyCount = 0;
			
			this->coordsLoaded = false;
			this->dataLoaded = false;
			this->changedData = false;
			this->shipsLoaded = false;
			this->shipsChanged = false;
			this->shipsSave = false;
			this->techsAdded = false;
			this->buildingsLoaded = false;
			
			this->buildingAtWork = "";
			this->actionName = "";
			this->userId = 0;
		}
		
		
		virtual ~Entity() {}
		virtual void saveData() = 0;
				
		int getId();
		char getCode();
		int getUserId();
		
		User* getUser();
		
		short getTypeId();
		bool getIsUserMain();
		
		void addMessageUser(Message* message);
		
		std::string getCoords();
		
		int getAbsX();
		int getAbsY();
		
		void setAction(std::string actionName);
		
		double getResMetal();
		double getResCrystal();
		double getResPlastic();
		double getResFuel();
		double getResFood();
		double getResPower();
		double getResPeople();
		double getResSum();
		
		void addResMetal(double metal);
		void addResCrystal(double crystal);
		void addResPlastic(double plastic);
		void addResFuel(double fuel);
		void addResFood(double food);
		void addResPower(double power);
		void addResPeople(double people);
		
		double removeResMetal(double metal);
		double removeResCrystal(double crystal);
		double removeResPlastic(double plastic);
		double removeResFuel(double fuel);
		double removeResFood(double food);
		double removeResPower(double power);
		double removeResPeople(double people);
		
		double getWfMetal();
		double getWfCrystal();
		double getWfPlastic();
		double getWfSum();
		
		void addWfMetal(double metal);
		void addWfCrystal(double crystal);
		void addWfPlastic(double plastic);
		
		double getObjectWfMetal(bool total=false);
		double getObjectWfCrystal(bool total=false);
		double getObjectWfPlastic(bool total=false);
		
		double getAddedWfMetal();
		double getAddedWfCrystal();
		double getAddedWfPlastic();
		
		double removeWfMetal(double metal);
		double removeWfCrystal(double crystal);
		double removeWfPlastic(double plastic);
		
		std::string getResString();
		
		void invadeEntity(int userId);
		void resetEntity(int userId=0);
		
		double getWeapon(bool total=false);
		double getShield(bool total=false);
		double getStructure(bool total=false);
		double getStructShield(bool total=false);
		double getHeal(bool total=false);
		double getInitCount(bool total=false);
		double getCount(bool total=false);
		double getDefCount();
		double getHealCount(bool total=false);
		double getSpyCount();
		
		void setPercentSurvive(double percentage, bool total=false);
		
		double addExp(double exp);
		double getExp();
		double getAddedExp();
		
		std::string bombBuilding(int Level);
		std::string empBuilding(int h);
		
		std::string getUserNicks();
		std::string getShieldString(bool small=true);
		std::string getStructureString(bool small=true);
		std::string getStructureShieldString();
		std::string getWeaponString(bool small=true);
		std::string getCountString(bool small=true);
		std::string getShipString();
		std::string getDefString(bool rebuild=false);
		
		std::string getBuildingString();
		
		std::string getLogResStart();
		std::string getLogResEnd();
		std::string getLogShipsStart();
		std::string getLogShipsEnd();
		
	protected:
		int id;
		int userId;
		int cellId;
		int sx,sy,cx,cy;
		short pos;
		char code;
		short typeId;
		
		User *entityUser;
		std::vector<Object*> objects;
		std::vector<Object*> specialObjects;
		std::vector<Object*> def;
		std::vector<Fleet*> fleets;
		std::map<std::string, int> buildings;
		
		double resMetal, resCrystal, resPlastic, resFuel, resFood, resPower, resPeople;
		double initResMetal, initResCrystal, initResPlastic, initResFuel, initResFood, initResPower, initResPeople;
		double wfMetal, wfCrystal, wfPlastic;
		double initWfMetal, initWfCrystal, initWfPlastic;
		
		int lastVisited, userChanged;
		std::string codeName;
		std::string coordsString;
		std::string actionName;
		
		std::string buildingAtWork;
		
		bool userMain;
		bool showCoords, coordsLoaded;
		bool dataLoaded;
		bool changedData;
		bool shipsLoaded, defLoaded;
		bool shipsChanged, shipsSave;
		bool techsAdded;
		bool buildingsLoaded;
		
		double initWeapon, initShield, initStructure, initStructShield, initHeal, initCount;
		double weapon, shield, structure, heal, count, healCount, spyCount, defCount;
		
		double exp;
		
		std::string logEntityShipStart, logEntityDefStart;

		void loadCoords();
		void loadAdditionalFleets();
		void loadShips();
		void recalcShips();
		void loadDef();
		void recalcDef();
		
		void addTechs();
		double getWeaponBonus();
		double getShieldBonus();
		double getStructureBonus();
		double getHealBonus();
		
		void loadBuildings();
		
		virtual void loadData() = 0;
};

#endif
