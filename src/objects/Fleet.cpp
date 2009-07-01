#include "Fleet.h"

	Fleet::Fleet(mysqlpp::Row &fleet) {
		this->action = std::string(fleet["action"]);
		this->fId = (int)fleet["id"];
		this->userId = (int)fleet["user_id"];
		this->leaderId = (int)fleet["leader_id"];
		this->entityFrom = (int)fleet["entity_from"];
		this->entityTo = (int)fleet["entity_to"];
		this->nextId = (int)fleet["next_id"];
		this->launchtime = (int)fleet["launchtime"];
		this->landtime = (int)fleet["landtime"];
		this->nextactiontime = (int)fleet["nextactiontime"];
		this->status = (short)fleet["status"];
		this->pilots = (double)fleet["pilots"];
		this->usageFuel = (int)fleet["usage_fuel"];
		this->usageFood = (int)fleet["usage_food"];
		this->usagePower = (int)fleet["usage_power"];
		this->supportUsageFuel = (int)fleet["support_usage_fuel"];
		this->supportUsageFood = (int)fleet["support_usage_food"];
		this->resMetal = (double)fleet["res_metal"];
		this->resCrystal = (double)fleet["res_crystal"];
		this->resPlastic = (double)fleet["res_plastic"];
		this->resFuel = (double)fleet["res_fuel"];
		this->resFood = (double)fleet["res_food"];
		this->resPower = (double)fleet["res_power"];
		this->resPeople = (double)fleet["res_people"];
		this->fetchMetal = (double)fleet["fetch_metal"];
		this->fetchCrystal = (double)fleet["fetch_crystal"];
		this->fetchPlastic = (double)fleet["fetch_plastic"];
		this->fetchFuel = (double)fleet["fetch_fuel"];
		this->fetchFood = (double)fleet["fetch_food"];
		this->fetchPower = (double)fleet["fetch_power"];
		this->fetchPeople = (double)fleet["fetch_people"];

		if (this->getLeaderId() == this->getId())
			this->loadAdditionalFleets();

		this->initResMetal = this->getResMetal();
		this->initResCrystal = this->getResCrystal();
		this->initResPlastic = this->getResPlastic();
		this->initResFuel = this->getResFuel();
		this->initResFood = this->getResFood();
		this->initResPower = this->getResPower();
		this->initResPeople = this->getResPeople();

		this->count = 0;
		this->weapon = 0;
		this->shield = 0;
		this->structure = 0;
		this->heal = 0;
		this->healCount = 0;
		this->actionCount = 0;

		this->initWeapon = -1;
		this->initShield = -1;
		this->initStructure = -1;
		this->initStructShield = -1;
		this->initHeal = -1;
		this->initCount = -1;
		
		this->allianceWeapon = 0;
		this->allianceStructure = 0;
		this->allianceShield = 0;

		this->exp = 0;

		this->antraxBonus = 0;
		this->antraxFoodBonus = 0;
		this->destroyBonus = 0;
		this->empBonus = 0;
		this->forstealBonus = 0;

		this->capacity = 0;
		this->peopleCapacity = 0;
		this->actionCapacity = 0;

		this->actionAllowed = false;
		this->shipsLoaded = false;
		this->entityLoaded = false;
		this->shipsChanged = false;

		this->techsAdded = false;
		this->allianceTechsLoaded = false;

		this->fleetUser = new User(this->getUserId());

		this->logFleetShipStart = "0";

		if (this->status==0) {
			this->usageFuel /= 2;
			this->usageFood /= 2;
			this->usagePower /= 2;
		}
		else if (this->status==3) {
			this->supportUsageFuel = 0;
			this->supportUsageFood = 0;
		}
		else {
			this->usageFuel = 0;
			this->usageFood = 0;
			this->usagePower = 0;
		}
	}

	int Fleet::getId() {
		return this->fId;
	}

	int Fleet::getUserId() {
		return this->userId;
	}

	int Fleet::getLeaderId() {
		return this->leaderId;
	}

	int Fleet::getEntityFrom() {
		return this->entityFrom;
	}

	int Fleet::getEntityTo() {
		return this->entityTo;
	}

	int Fleet::getNextId() {
		return this->nextId;
	}

	int Fleet::getLandtime() {
		return this->landtime;
	}

	int Fleet::getLaunchtime() {
		return this->launchtime;
	}

	int Fleet::getNextactiontime() {
		return this->nextactiontime;
	}

	std::string Fleet::getAction(bool blank) {
		if (blank)
			return this->action;
		else {
			Config &config = Config::instance();
			std::string action = config.getActionName(this->action);
			if (this->status==1)
				action += " (Rückflug)";
			else if (this->status==2)
				action += " (Abgebrochen)";
			return action;
		}
	}

	short Fleet::getStatus() {
		return this->status;
	}

	void Fleet::addMessageUser(Message* message) {
		message->addUserId(this->getUserId());
		std::string nicks = this->fleetUser->getUserNick();
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			std::size_t found;
			for ( it=fleets.begin() ; it < fleets.end(); it++ ) {
				std::string key = (*it)->fleetUser->getUserNick();
				found=nicks.rfind(key);
				if (found==std::string::npos) {
					(*it)->addMessageUser(message);
					nicks += ", "
							+ key;
				}
			}
		}
	}

	double Fleet::getPilots(bool total) {
		double pilots = this->pilots;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				pilots += (*it)->getPilots(total);
		}
		return pilots;
	}

	double Fleet::getResMetal(bool total) {
		double resMetal = this->resMetal;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				resMetal += (*it)->getResMetal(total);
		}
		return resMetal;
	}

	double Fleet::getResCrystal(bool total) {
		double resCrystal = this->resCrystal;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				resCrystal += (*it)->getResCrystal(total);
		}
		return resCrystal;
	}

	double Fleet::getResPlastic(bool total) {
		double resPlastic = this->resPlastic;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				resPlastic += (*it)->getResPlastic(total);
		}
		return resPlastic;
	}

	double Fleet::getResFuel(bool total) {
		double resFuel = this->resFuel;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				resFuel += (*it)->getResFuel(total);
		}
		return resFuel;
	}

	double Fleet::getResFood(bool total) {
		double resFood = this->resFood;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				resFood += (*it)->getResFood(total);
		}

		return resFood;
	}

	double Fleet::getResPower(bool total) {
		double resPower = this->resPower;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				resPower += (*it)->getResPower(total);
		}
		return resPower;
	}

	double Fleet::getResPeople(bool total) {
		double resPeople = this->resPeople;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				resPeople += (*it)->getResPeople(total);
		}
		return resPeople;
	}

	double Fleet::getResLoaded(bool total) {
		return this->getResMetal(total)
				+ this->getResCrystal(total)
				+ this->getResPlastic(total)
				+ this->getResFuel(total)
				+ this->getResFood(total);
	}

	double Fleet::getInitResLoaded() {
		return this->initResMetal
				+ this->initResCrystal
				+ this->initResPlastic
				+ this->initResFuel
				+ this->initResFood;
	}

	double Fleet::getCapacity(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		double capacity = this->capacity - this->getResLoaded() - this->usageFuel - this->usageFood - this->supportUsageFuel - this->supportUsageFood;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				capacity += (*it)->getCapacity(total);
		}
		return capacity;
	}

	double Fleet::getCapa() {
		return this->capacity;
	}

	double Fleet::getActionCapacity(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		double actionCapacity = std::min(this->actionCapacity,this->getCapacity());

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				actionCapacity += (*it)->getActionCapacity(total);
		}
		return actionCapacity;
	}

	double Fleet::getPeopleCapacity(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		double peopleCapacity = 0;
		peopleCapacity += this->peopleCapacity;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				peopleCapacity += (*it)->getPeopleCapacity(total);
		}
		return peopleCapacity;
	}

	double Fleet::getBounty() {
		return this->bounty;
	}

	double Fleet::getBountyBonus(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		double bounty = getBounty();
		double capacity = getCapa();

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ ) {
				capacity += (*it)->getCapa();
				bounty += (*it)->getBounty();
			}
		}
		return bounty/capacity;
	}

	void Fleet::addRaidedRes() {
		this->fleetUser->addRaidedRes(this->getResLoaded()-this->getInitResLoaded());

		if (this->fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=this->fleets.begin() ; it < this->fleets.end(); it++ )
				(*it)->fleetUser->addRaidedRes((*it)->getResLoaded()-(*it)->getInitResLoaded());
		}
	}

	double Fleet::getFetchMetal(bool total) {
		double fetchMetal = this->fetchMetal;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				fetchMetal += (*it)->getFetchMetal(total);
		}
		return fetchMetal;
	}

	double Fleet::getFetchCrystal(bool total) {
		double fetchCrystal = this->fetchCrystal;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				fetchCrystal += (*it)->getFetchCrystal(total);
		}
		return fetchCrystal;
	}

	double Fleet::getFetchPlastic(bool total) {
		double fetchPlastic = this->fetchPlastic;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				fetchPlastic += (*it)->getFetchPlastic(total);
		}
		return fetchPlastic;
	}

	double Fleet::getFetchFuel(bool total) {
		double fetchFuel = this->fetchFuel;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				fetchFuel += (*it)->getFetchFuel(total);
		}
		return fetchFuel;
	}

	double Fleet::getFetchFood(bool total) {
		double fetchFood = this->fetchFood;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				fetchFood += (*it)->getFetchFood(total);
		}
		return fetchFood;
	}

	double Fleet::getFetchPeople(bool total) {
		double fetchPeople = this->fetchPeople;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				fetchPeople += (*it)->getFetchPeople(total);
		}
		return fetchPeople;
	}

	double Fleet::getFetchSum(bool total) {
		return(this->getFetchMetal(total) + this->getFetchCrystal(total) + this->getFetchPlastic(total) + this->getFetchFuel(total) + this->getFetchFood(total));
	}

	double Fleet::addMetal(double metal, bool total) {
		this->changedData = true;
		metal = round(metal);
		if (metal>=this->getCapacity(total))
			metal = this->getCapacity(total);
		
		if (metal*this->getCapacity(total))
			this->resMetal += metal*this->getCapacity()/this->getCapacity(total);
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				metal += (*it)->addMetal(metal*(*it)->getCapacity()/this->getCapacity(total));
		}
		return metal;
	}

	double Fleet::addCrystal(double crystal, bool total) {
		crystal = round(crystal);
		this->changedData = true;
		if (crystal>=this->getCapacity())
			crystal = this->getCapacity();
		
		if (crystal*this->getCapacity(total))
			this->resCrystal += crystal*this->getCapacity()/this->getCapacity(total);
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				crystal += (*it)->addCrystal(crystal*(*it)->getCapacity()/this->getCapacity(total));
		}
		return crystal;
	}

	double Fleet::addPlastic(double plastic, bool total) {
		plastic = round(plastic);
		this->changedData = true;
		if (plastic>=this->getCapacity())
			plastic = this->getCapacity();
		
		if (plastic*this->getCapacity(total))
			this->resPlastic += plastic*this->getCapacity()/this->getCapacity(total);
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				plastic += (*it)->addPlastic(plastic*(*it)->getCapacity()/this->getCapacity(total));
		}
		return plastic;
	}

	double Fleet::addFuel(double fuel, bool total) {
		fuel = round(fuel);
		this->changedData = true;
		if (fuel>=this->getCapacity())
			fuel = this->getCapacity();
		
		if (fuel*this->getCapacity(total))
			this->resFuel += fuel*this->getCapacity()/this->getCapacity(total);
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				fuel += (*it)->addFuel(fuel*(*it)->getCapacity()/this->getCapacity(total));
		}
		return fuel;
	}

	double Fleet::addFood(double food, bool total) {
		food = round(food);
		this->changedData = true;
		if (food>=this->getCapacity())
			food = this->getCapacity();
		
		if (food*this->getCapacity(total))
			this->resFood += food*this->getCapacity()/this->getCapacity(total);
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				food += (*it)->addFood(food*(*it)->getCapacity()/this->getCapacity(total));
		}
		return food;
	}

	double Fleet::addPower(double power, bool total) {
		power = round(power);
		this->changedData = true;
		if (power>=this->getCapacity())
			power = this->getCapacity();
		
		if (power*this->getCapacity(total))
			this->resPower += power*this->getCapacity()/this->getCapacity(total);
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				power += (*it)->addPower(power*(*it)->getCapacity()/this->getCapacity(total));
		}
		return power;
	}

	double Fleet::addPeople(double people, bool total) {
		people = round(people);
		this->changedData = true;
		if (people>=this->getPeopleCapacity())
			people = this->getPeopleCapacity();
		
		if (people*this->getPeopleCapacity(total))
			this->resPeople += people*this->getPeopleCapacity()/this->getPeopleCapacity(total);
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				people += (*it)->addPeople(people*(*it)->getPeopleCapacity()/this->getPeopleCapacity(total));
		}
		
		return people;
	}

	double Fleet::unloadResMetal() {
		this->changedData = true;
		double metal = this->resMetal;
		this->resMetal = 0;
		return metal;
	}

	double Fleet::unloadResCrystal() {
		this->changedData = true;
		double crystal = this->resCrystal;
		this->resCrystal = 0;
		return crystal;
	}

	double Fleet::unloadResPlastic() {
		this->changedData = true;
		double plastic = this->resPlastic;
		this->resPlastic = 0;
		return plastic;
	}

	double Fleet::unloadResFuel(bool land) {
		this->changedData = true;
		double fuel = this->resFuel;
		if (land) {
			fuel += this->usageFuel + this-> supportUsageFuel;
			this->usageFuel = 0;
			this->supportUsageFuel = 0;
		}
		this->resFuel = 0;
		return fuel;
	}

	double Fleet::unloadResFood(bool land) {
		this->changedData = true;
		double food = this->getResFood();
		if (land) {
			food += this->usageFood + this->supportUsageFood;
			this->usageFood = 0;
			this->supportUsageFood = 0;
		}
		this->resFood = 0;
		return food;
	}

	double Fleet::unloadResPower() {
		this->changedData = true;
		double power = this->resPower;
		this->resPower = 0;
		return power;
	}

	double Fleet::unloadResPeople(bool land) {
		this->changedData = true;
		double people = this->resPeople;
		if (land) {
			people += this->pilots;
			this->pilots = 0;
		}
		this->resPeople = 0;
		return people;
	}

	double Fleet::getWfMetal(bool total) {
		double wfMetal = 0;
		std::vector<Object*>::iterator ot;
		for (ot=objects.begin() ; ot < objects.end(); ot++)
			wfMetal += (*ot)->getWfMetal();
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				wfMetal += (*it)->getWfMetal();
		}

		return wfMetal;
	}

	double Fleet::getWfCrystal(bool total) {
		double wfCrystal = 0;
		std::vector<Object*>::iterator ot;
		for (ot=objects.begin() ; ot < objects.end(); ot++)
			wfCrystal += (*ot)->getWfCrystal();
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				wfCrystal += (*it)->getWfCrystal();
		}

		return wfCrystal;
	}

	double Fleet::getWfPlastic(bool total) {
		double wfPlastic = 0;
		std::vector<Object*>::iterator ot;
		for (ot=objects.begin() ; ot < objects.end(); ot++)
			wfPlastic += (*ot)->getWfPlastic();
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				wfPlastic += (*it)->getWfPlastic();
		}

		return wfPlastic;
	}

	double Fleet::getWeapon(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		if (!this->techsAdded)
			this->addTechs();
		double weapon = this->weapon;
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				weapon += (*it)->getWeapon(total);
		}
		return weapon;
	}

	double Fleet::getShield(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		if (!this->techsAdded)
			this->addTechs();
		double shield = this->shield;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				shield += (*it)->getShield(total);
		}
		return shield;
	}

	double Fleet::getStructure(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		if (!this->techsAdded)
			this->addTechs();
		double structure = this->structure;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				structure += (*it)->getStructure(total);
		}
		return structure;
	}

	double Fleet::getStructShield(bool total) {
		double structShield = this->getStructure() + this->getShield();

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				structShield += (*it)->getStructShield(total);
		}
		return structShield;
	}

	double Fleet::getHeal(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		if (!this->techsAdded)
			this->addTechs();
		double heal = this->heal;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				heal += (*it)->getHeal(total);
		}
		return heal;
	}

	double Fleet::getInitCount(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		double initCount = this->initCount;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				initCount += (*it)->getInitCount(total);
		}
		return initCount;
	}

	double Fleet::getCount(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		double count = this->count;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				count += (*it)->getCount(total);
		}
		return count;
	}

	double Fleet::getHealCount(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		double healCount = this->healCount;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				healCount += (*it)->getHealCount(total);
		}
		return healCount;
	}

	unsigned int Fleet::getActionCount(bool total) {
		if (!this->shipsLoaded)
			this->loadShips();
		if (this->shipsChanged)
			this->recalcShips();
		unsigned int actionCount = this->actionCount;

		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				actionCount += (*it)->getActionCount(total);
		}
		return actionCount;
	}

	double Fleet::addExp(double exp) {
		int counter = 0;
		std::vector<Object*>::iterator ot;
		for (ot = this->specialObjects.begin() ; ot < this->specialObjects.end(); ot++) {
			counter++;
			(*ot)->addExp(exp);
		}
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				counter += (int)(*it)->addExp(exp);
		}
		if (counter)
			this->exp += exp;
		else
			this->exp = -1;
			// TODO added this to overcome error, but which value should it be?
		return exp;
	}

	double Fleet::getExp() {
		double exp = 0;

		DataHandler &DataHandler = DataHandler::instance();

		std::vector<Object*>::iterator ot;
		for (ot = this->objects.begin() ; ot < this->objects.end(); ot++) {
			ShipData::ShipData *data = DataHandler.getShipById((*ot)->getTypeId());
			exp += ((*ot)->getInitCount() - (*ot)->getCount()) * data->getCosts();
		}
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				exp += (*it)->getExp();
		}
		return exp;
	}

	double Fleet::getAddedExp()
	{
		return this->exp;
	}

	double Fleet::getSpecialShipBonusAntrax() {
		double antraxBonus = this->antraxBonus;
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				antraxBonus += (*it)->getSpecialShipBonusAntrax();
		}
		return antraxBonus;
	}

	double Fleet::getSpecialShipBonusAntraxFood() {
		double antraxFoodBonus = this->antraxFoodBonus;
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				antraxFoodBonus += (*it)->getSpecialShipBonusAntraxFood();
		}
		return antraxFoodBonus;
	}

	double Fleet::getSpecialShipBonusBuildDestroy() {
		double destroyBonus = this->destroyBonus;
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				destroyBonus += (*it)->getSpecialShipBonusBuildDestroy();
		}
		return destroyBonus;
	}

	double Fleet::getSpecialShipBonusEMP() {
		double empBonus = this->empBonus;
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				empBonus += (*it)->getSpecialShipBonusEMP();
		}
		return empBonus;
	}

	double Fleet::getSpecialShipBonusForsteal() {
		double forstealBonus = this->forstealBonus;
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				forstealBonus += (*it)->getSpecialShipBonusForsteal();
		}
		return forstealBonus;
	}

	void Fleet::deleteActionShip(int count) {
		this->shipsChanged = true;
		std::vector<Object*>::iterator ot;
		for (ot = this->actionObjects.begin() ; ot < this->actionObjects.end(); ot++) {
			count = (*ot)->removeObjects(count);
			if (!count) break;
		}
	}

	void Fleet::setPercentSurvive(double percentage, bool total) {
		percentage = std::max(percentage,0.0);
		std::vector<Object*>::iterator ot;
		for (ot = this->objects.begin() ; ot < this->objects.end(); ot++)
			(*ot)->setPercentSurvive(percentage);
		if (total && fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				(*it)->setPercentSurvive(percentage);
		}

		this->shipsChanged = true;
	}

	void Fleet::setReturn() {
		int entity = this->entityFrom;
		this->entityFrom = this->entityTo;
		int duration;

		if (this->getStatus() == 3 && (this->getNextactiontime() > 0 || this->getAction()=="support")) {
			duration = this->getNextactiontime();
			this->entityTo = this->getNextId();
		}
		else {
			duration = this->getLandtime() - this->getLaunchtime();
			this->entityTo = entity;
		}
		this->launchtime = this->getLandtime();
		this->landtime = this->getLaunchtime() + duration;

		this->status = 1;

		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ )
				(*it)->setReturn();
		}
	}

	void Fleet::setMain() {
		My &my = My::instance();
		mysqlpp::Connection *con = my.get();
		mysqlpp::Query query = con->query();
		query << "SELECT ";
		query << "	planets.id ";
		query << "FROM ";
		query << "	planets ";
		query << "WHERE ";
		query << "	planets.planet_user_id='" << this->getUserId() << "' ";
		query << "	AND planets.planet_user_main='1' ";
		query << "LIMIT 1;";
		mysqlpp::Result mainRes = query.store();
		query.reset();

		if (mainRes) {
			int mainSize = mainRes.size();

			if (mainSize > 0) {
				mysqlpp::Row mainRow = mainRes.at(0);

				int duration = this->getLandtime() - this->getLaunchtime();
				this->launchtime = this->getLandtime();
				this->landtime += duration;
				this->status = 2;
				this->entityFrom = this->entityTo;
				this->entityTo = (int)mainRow["id"];
			}
		}
	}

	void Fleet::setSupport() {
		int temp = this->getLandtime() - this->getLaunchtime();
		this->launchtime = this->getLandtime();
		this->landtime = this->getLandtime() + this->getNextactiontime();
		this->status = 3;
		this->nextactiontime = temp;

		this->nextId = this->entityFrom;
		this->entityFrom = this->entityTo;
	}

	std::string Fleet::getActionString() {
		return this->getAction();
	}

	std::string Fleet::getLandtimeString() {
		return etoa::formatTime(this->getLandtime());
	}

	std::string Fleet::getLaunchtimeString() {
		return  etoa::formatTime(this->getLaunchtime());
	}

	std::string Fleet::getUserNicks() {
		std::string nicks = this->fleetUser->getUserNick();
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			std::size_t found;
			for ( it=fleets.begin() ; it < fleets.end(); it++ ) {
				std::string key = (*it)->fleetUser->getUserNick();
				found=nicks.rfind(key);
				if (found==std::string::npos)
					nicks += ", "
							+ key;
			}
		}
		return nicks;
	}

	std::string Fleet::getUserIds() {
		std::string ids = "," + etoa::d2s(this->getUserId()) + ",";
		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			std::size_t found;
			for ( it=fleets.begin() ; it < fleets.end(); it++ ) {
				std::string key = "," + etoa::d2s((*it)->getUserId()) + ",";
				found=ids.rfind(key);
				if (found==std::string::npos)
					ids += ","
							+ key;
			}
		}
		return ids;
	}

	std::string Fleet::getShieldString(bool small) {
		if (!this->allianceTechsLoaded)
			this->loadAllianceTechs();
		std::string shieldString = "";
		if (!small) {
			int counter = 1;
			double shieldTech = this->getShieldBonus();
			shieldString += "[b]Schild (";
			if (fleets.size()) {
				shieldString += "~";
				std::vector<Fleet*>::iterator it;
				for ( it=fleets.begin() ; it < fleets.end(); it++ ) {
					counter++;
					shieldTech += (*it)->getShieldBonus();
				}
			}

			shieldString += etoa::d2s(round(shieldTech*100/counter));
			shieldString += "%):[/b] ";
		}
		shieldString += etoa::nf(etoa::d2s(this->getShield(true)));

		return shieldString;
	}

	std::string Fleet::getStructureString(bool small) {
		if (!this->allianceTechsLoaded)
			this->loadAllianceTechs();
		std::string structureString = "";
		if (!small) {
			int counter = 1;
			double structureTech = this->getStructureBonus();
			structureString += "[b]Struktur (";
			if (fleets.size()) {
				structureString += "~";
				std::vector<Fleet*>::iterator it;
				for ( it=fleets.begin() ; it < fleets.end(); it++ ) {
					counter++;
					structureTech += (*it)->getStructureBonus();
				}
			}

			structureString += etoa::d2s(round(structureTech*100/counter));
			structureString += "%):[/b] ";
		}
		structureString += etoa::nf(etoa::d2s(this->getStructure(true)));

		return structureString;
	}

	std::string Fleet::getStructureShieldString() {
		return etoa::nf(etoa::d2s(getStructShield(true)));
	}

	std::string Fleet::getWeaponString(bool small) {
		if (!this->allianceTechsLoaded)
			this->loadAllianceTechs();
		std::string weaponString = "";
		if (!small) {
			int counter = 1;
			double weaponTech = this->getWeaponBonus();
			weaponString += "[b]Waffen (";
			if (fleets.size()) {
				weaponString += "~";
				std::vector<Fleet*>::iterator it;
				for ( it=fleets.begin() ; it < fleets.end(); it++ ) {
					counter++;
					weaponTech += (*it)->getWeaponBonus();
				}
			}

			weaponString += etoa::d2s(round(weaponTech*100/counter));
			weaponString += "%):[/b] ";
		}
		weaponString += etoa::nf(etoa::d2s(this->getWeapon(true)));

		return weaponString;
	}

	std::string Fleet::getCountString(bool small) {
		std::string countString = "";
		if (!small) {
			countString += "[b]Einheiten:[/b] ";
		}
		countString += etoa::nf(etoa::d2s(this->getCount(true)));
		return countString;
	}

	std::string Fleet::getDestroyedShipString(std::string reason) {
		std::string destroyedString = "";

		DataHandler &DataHandler = DataHandler::instance();
		std::vector<Object*>::iterator it;
		for (it = this->objects.begin() ; it < this->objects.end(); it++) {
			if ((*it)->getCount() < (*it)->getInitCount()) {
				ShipData::ShipData *data = DataHandler.getShipById((*it)->getTypeId());
				destroyedString +=  etoa::d2s((*it)->getInitCount() - (*it)->getCount())
								+ " "
								+ data->getName()
								+ "\n";
			}
		}

		if (destroyedString.length()>0)
			destroyedString = reason + destroyedString;

		return destroyedString;
	}

	std::string Fleet::getResCollectedString(bool total, std::string subject) {
		std::string msgRes = "\n\n\n[b]"
							+ subject
							+ ":[/b]\n\nTitan: "
							+ etoa::nf(etoa::d2s(this->getResMetal(total) - this->initResMetal))
							+ "\nSilizium: "
							+ etoa::nf(etoa::d2s(this->getResCrystal(total) - this->initResCrystal))
							+ "\nPVC: "
							+ etoa::nf(etoa::d2s(this->getResPlastic(total) - this->initResPlastic))
							+ "\nTritium: "
							+ etoa::nf(etoa::d2s(this->getResFuel(total) - this->initResFuel))
							+ "\nNahrung: "
							+ etoa::nf(etoa::d2s(this->getResFood(total) - this->initResFood))
							+ "\nBewohner: "
							+ etoa::nf(etoa::d2s(this->getResPeople(total) - this->initResPeople))
							+ "\n";
		return msgRes;
	}

	std::string Fleet::getShipString() {
		if (!this->shipsLoaded)
			this->loadShips();
		std::map<int,int> ships;
		std::map<int,int> specialShips;

		std::vector<Object*>::iterator ot;
		for (ot = this->objects.begin() ; ot < this->objects.end(); ot++) {
			if ((*ot)->getSpecial())
				specialShips[(*ot)->getTypeId()] += (*ot)->getCount();
			else
				ships[(*ot)->getTypeId()] += (*ot)->getCount();
		}

		if (fleets.size()) {
			std::vector<Fleet*>::iterator it;
			for ( it=fleets.begin() ; it < fleets.end(); it++ ) {
				for (ot = (*it)->objects.begin() ; ot < (*it)->objects.end(); ot++) {
					if ((*ot)->getSpecial())
						specialShips[(*ot)->getTypeId()] += (*ot)->getCount();
					else
						ships[(*ot)->getTypeId()] += (*ot)->getCount();
				}
			}
		}
		std::string shipString = "";

		DataHandler &DataHandler = DataHandler::instance();
		std::map<int,int>::iterator st;
		for ( st=specialShips.begin() ; st != specialShips.end(); st++ ) {
			ShipData::ShipData *data = DataHandler.getShipById((*st).first);
			shipString += "[tr][td]"
						+ data->getName()
						+ "[/td][td]"
						+ etoa::nf(etoa::d2s((*st).second))
						+ "[/td][/tr]";
		}
		for ( st=ships.begin() ; st != ships.end(); st++ ) {
			ShipData::ShipData *data = DataHandler.getShipById((*st).first);
			shipString += "[tr][td]"
						+ data->getName()
						+ "[/td][td]"
						+ etoa::nf(etoa::d2s((*st).second))
						+ "[/td][/tr]";
		}
		if (shipString.length()<1)
			shipString = "[i]Nichts vorhanden![/i]\n";
		else
			shipString = "[table]" + shipString + "[/table]";
		return shipString;

	}


	bool Fleet::actionIsAllowed() {
		if (!this->shipsLoaded)
			this->loadShips();
		else if (this->shipsChanged)
			this->recalcShips();
		return this->actionAllowed;
	}

	void Fleet::loadAdditionalFleets() {
		My &my = My::instance();
		mysqlpp::Connection *con = my.get();

		std::time_t time = std::time(0);

		mysqlpp::Query query = con->query();
		query << "SELECT ";
		query << " * ";
		query << "FROM ";
		query << " fleet ";
		query << "WHERE ";
		query << "	leader_id='" << this->getLeaderId() << "' ";
		query << "	AND landtime<='" << time << "' ";
		query << "	AND status=3;";
		mysqlpp::Result fRes = query.store();
		query.reset();

		if (fRes) {
			int fSize = fRes.size();

			if (fSize>0) {
				mysqlpp::Row fRow;
				Fleet* additionalFleet;
				for (int i=0; i<fSize; i++) {
					fRow = fRes.at(i);
					additionalFleet = new Fleet(fRow);
					fleets.push_back(additionalFleet);
				}
			}
		}
	}

	void Fleet::loadShips() {
		if (!this->shipsLoaded) {
			this->shipsLoaded = true;
			Config &config = Config::instance();
			My &my = My::instance();
			mysqlpp::Connection *con = my.get();

			mysqlpp::Query query = con->query();
			query << "SELECT ";
			query << "	* ";
			query << "FROM ";
			query << "	fleet_ships ";
			query << "WHERE ";
			query << "	fs_fleet_id='" << this->getId() << "';";
			mysqlpp::Result fsRes = query.store();
			query.reset();

			if (fsRes) {
				int fsSize = fsRes.size();

				if (fsSize>0) {
					this->logFleetShipStart = "";

					DataHandler &DataHandler = DataHandler::instance();
					mysqlpp::Row fsRow;

					for (int i=0; i<fsSize; i++) {
						fsRow = fsRes.at(i);

						if (config.idget("MARKET_SHIP_ID")!=(int)fsRow["fs_ship_id"]) {
							Object* object = ObjectFactory::createObject(fsRow, 'f');
							ShipData::ShipData *data = DataHandler.getShipById(object->getTypeId());

							this->capacity += object->getCount() * data->getCapacity();
							this->peopleCapacity += object->getCount() * data->getPeopleCapacity();

							this->bounty += object->getCount() * data->getCapacity() * data->getBountyBonus();
							if (data->getActions(this->action)) {
								this->actionAllowed = true;
								this->actionCapacity += object->getCount() * data->getCapacity();
								this->actionCount += object->getCount();
								this->actionObjects.push_back(object);
							}

							this->count += object->getCount();
							this->weapon += object->getCount() * data->getWeapon();
							this->shield += object->getCount() * data->getShield();
							this->structure += object->getCount() * data->getStructure();
							this->heal += object->getCount() * data->getHeal();
							if (data->getHeal()>0)
								this->healCount += object->getCount();

							this->logFleetShipStart += etoa::d2s(object->getTypeId())
													+ ":"
													+ etoa::d2s(object->getCount())
													+ ",";

							this->objects.push_back(object);

							if (object->getSpecial()) {
								this->antraxBonus += object->getCount() * object->getSBonusAntrax() * data->getBonusAntrax();
								this->antraxFoodBonus += object->getCount() * object->getSBonusAntraxFood() * data->getBonusAntraxFood();
								this->destroyBonus += object->getCount() * object->getSBonusBuildDestroy() * data->getBonusBuildDestroy();
								this->empBonus += object->getCount() * object->getSBonusDeactivade() * data->getBonusDeactivade();
								this->forstealBonus += object->getCount() * object->getSBonusForsteal() * data->getBonusForsteal();
								this->specialObjects.push_back(object);
							}
						}
					}
				}
			}
			if (fleets.size()) {
				std::vector<Fleet*>::iterator it;
				for ( it=fleets.begin() ; it < fleets.end(); it++ )
					(*it)->loadShips();
			}
		}
	}

	void Fleet::recalcShips() {
		if (this->shipsChanged) {
			this->shipsChanged = false;
			this->actionCapacity = 0;
			this->capacity = 0;
			this->peopleCapacity = 0;

			this->count = 0;
			this->weapon = 0;
			this->shield = 0;
			this->structure = 0;
			this->heal = 0;
			this->bounty = 0;
			this->actionCount = 0;

				this->antraxBonus = 0;
				this->antraxFoodBonus = 0;
				this->destroyBonus = 0;
				this->empBonus = 0;
				this->forstealBonus = 0;

			this->techsAdded = false;

			this->actionAllowed = false;

			DataHandler &DataHandler = DataHandler::instance();

			std::vector<Object*>::iterator it;
			for (it=objects.begin() ; it < objects.end(); it++) {
				ShipData::ShipData *data = DataHandler.getShipById((*it)->getTypeId());

				this->capacity += (*it)->getCount() * data->getCapacity();
				this->peopleCapacity += (*it)->getCount() * data->getPeopleCapacity();

				this->bounty += (*it)->getCount() * data->getCapacity() * data->getBountyBonus();

				if (data->getActions(this->action)) {
					this->actionAllowed = true;
					this->actionCapacity += (*it)->getCount() * data->getCapacity();
					this->actionCount += (*it)->getCount();
				}

				this->count += (*it)->getCount();
				this->weapon += (*it)->getCount() * data->getWeapon();
				this->shield += (*it)->getCount() * data->getShield();
				this->structure += (*it)->getCount() * data->getStructure();
				this->heal += (*it)->getCount() * data->getHeal();

				if ((*it)->getSpecial())
					this->antraxBonus += (*it)->getCount() * (*it)->getSBonusAntrax() * data->getBonusAntrax();
					this->antraxFoodBonus += (*it)->getCount() * (*it)->getSBonusAntraxFood() * data->getBonusAntraxFood();
					this->destroyBonus += (*it)->getCount() * (*it)->getSBonusBuildDestroy() * data->getBonusBuildDestroy();
					this->empBonus += (*it)->getCount() * (*it)->getSBonusDeactivade() * data->getBonusDeactivade();
					this->forstealBonus += (*it)->getCount() * (*it)->getSBonusForsteal() * data->getBonusForsteal();
			}

			if (fleets.size()) {
				std::vector<Fleet*>::iterator it;
				for ( it=fleets.begin() ; it < fleets.end(); it++ )
					(*it)->recalcShips();
			}
		}
	}

	void Fleet::addTechs() {
		if (!this->techsAdded) {
			if (!this->allianceTechsLoaded)
				this->loadAllianceTechs();
			this->techsAdded = true;
			this->weapon *= this->getWeaponBonus();
			this->shield *= this->getShieldBonus();
			this->structure *= this->getStructureBonus();
			this->heal *= this->getHealBonus();
		}
		if (this->initWeapon<0)
			this->initWeapon = this->weapon;
		if (this->initShield<0)
			this->initShield = this->shield;
		if (this->initStructure<0)
			this->initStructure = this->structure;
		if (this->initStructShield<0)
			this->initStructShield = this->initStructure + this->initShield;
		if (this->initHeal<0)
			this->initHeal = this->heal;
		if (this->initCount<0)
			this->initCount = this->count;
	}
	
	void Fleet::loadAllianceTechs() {
		if (this->fleetUser->getAllianceId()!=0) {
			My &my = My::instance();
			mysqlpp::Connection *con = my.get();
			mysqlpp::Query query = con->query();
			query << "SELECT "
				<< "	alliance_techlist_tech_id, "
				<< "	alliance_techlist_current_level "
				<< "FROM "
				<< "	alliance_techlist "
				<< "WHERE "
				<< "	alliance_techlist_alliance_id='" << this->fleetUser->getAllianceId() << "';";
			mysqlpp::Result aRes = query.store();
			query.reset();
			
			if (aRes) {
				Config &config = Config::instance();
				int aSize = aRes.size();
				
				std::string users = this->getUserNicks();
				size_t found;
				int userCount = 0;
				found=users.find_first_of(",");
				while (found!=std::string::npos)
				{
					userCount++;
					found=users.find_first_of(",",found+1);
				}
				
				if (aSize>0) {
					mysqlpp::Row aRow;
					for (int i=0; i<aSize; i++) {
						aRow = aRes.at(i);
						if ((int)aRow["alliance_techlist_tech_id"]==5)
							this->allianceWeapon = ((int)config.nget("alliance_tech_bonus",0) * (int)aRow["alliance_techlist_current_level"])
							+ (int)((int)config.nget("alliance_tech_bonus",1) * userCount);
						if ((int)aRow["alliance_techlist_tech_id"]==6)
							this->allianceShield = ((int)config.nget("alliance_tech_bonus",0) * (int)aRow["alliance_techlist_current_level"])
							+ (int)((int)config.nget("alliance_tech_bonus",1) * userCount);
						if ((int)aRow["alliance_techlist_tech_id"]==7)
							this->allianceStructure = ((int)config.nget("alliance_tech_bonus",0) * (int)aRow["alliance_techlist_current_level"])
							+ (int)((int)config.nget("alliance_tech_bonus",1) * userCount);
					}
					if (fleets.size()) {
						std::vector<Fleet*>::iterator it;
						for ( it=fleets.begin() ; it < fleets.end(); it++ ) {
							(*it)->setAllianceWeapon(this->allianceWeapon);
							(*it)->setAllianceShield(this->allianceShield);
							(*it)->setAllianceStructure(this->allianceStructure);
						}
					}
				}
			}
		}
		this->allianceTechsLoaded = true;
	}
			

	double Fleet::getWeaponBonus() {
		double bonus = 1;
		if (specialObjects.size()) {
			std::vector<Object*>::iterator it;
			DataHandler &DataHandler = DataHandler::instance();
			for ( it=specialObjects.begin() ; it < specialObjects.end(); it++ ) {
				ShipData::ShipData *data = DataHandler.getShipById((*it)->getTypeId());
				bonus += (*it)->getSBonusWeapon() * data->getBonusWeapon();
			}
		}
		bonus += this->fleetUser->getTechBonus("Waffentechnik") + this->allianceWeapon/100.0;
		return bonus;

	}

	double Fleet::getShieldBonus() {
		double bonus = 1;
		if (specialObjects.size()) {
			std::vector<Object*>::iterator it;
			DataHandler &DataHandler = DataHandler::instance();
			for ( it=specialObjects.begin() ; it < specialObjects.end(); it++ ) {
				ShipData::ShipData *data = DataHandler.getShipById((*it)->getTypeId());
				bonus += (*it)->getSBonusShield() * data->getBonusShield();
			}
		}
		bonus += this->fleetUser->getTechBonus("Schutzschilder") + this->allianceShield/100.0;
		return bonus;
	}

	double Fleet::getStructureBonus() {
		double bonus = 1;
		if (specialObjects.size()) {
			std::vector<Object*>::iterator it;
			DataHandler &DataHandler = DataHandler::instance();
			for ( it=specialObjects.begin() ; it < specialObjects.end(); it++ ) {
				ShipData::ShipData *data = DataHandler.getShipById((*it)->getTypeId());
				bonus += (*it)->getSBonusStructure() * data->getBonusStructure();
			}
		}
		bonus += this->fleetUser->getTechBonus("Panzerung") + this->allianceStructure/100.0;
		return bonus;
	}

	double Fleet::getHealBonus() {
		double bonus = 1;
		if (specialObjects.size()) {
			std::vector<Object*>::iterator it;
			DataHandler &DataHandler = DataHandler::instance();
			for ( it=specialObjects.begin() ; it < specialObjects.end(); it++ ) {
				ShipData::ShipData *data = DataHandler.getShipById((*it)->getTypeId());
				bonus += (*it)->getSBonusHeal() * data->getBonusHeal();
			}
		}
		bonus += this->fleetUser->getTechBonus("Regenatechnik");
		return bonus;
	}

	void Fleet::setAllianceWeapon(int weapon) {
		this->allianceWeapon = weapon;
		this->allianceTechsLoaded = true;
	}

	void Fleet::setAllianceStructure(int structure) {
		this->allianceStructure = structure;
		this->allianceTechsLoaded = true;
	}

	void Fleet::setAllianceShield(int shield) {
		this->allianceShield = shield;
		this->allianceTechsLoaded = true;
	}

	void Fleet::save() {
		int sum = 0;
		while (!objects.empty()) {
			Object* object = objects.back();
			sum += object->getCount();
			delete object;
			objects.pop_back();
		}

		while (!fleets.empty()) {
			Fleet* fleet = fleets.back();
			delete fleet;
			fleets.pop_back();
		}

		My &my = My::instance();
		mysqlpp::Connection *con = my.get();
		mysqlpp::Query query = con->query();

		if (sum>0 || !this->shipsLoaded) {
			query << "UPDATE ";
			query << "	fleet ";
			query << "SET ";
			query << "	entity_from='" << this->getEntityFrom() << "', ";
			query << "	entity_to='" << this->getEntityTo() << "', ";
			query << "	next_id='" << this->getNextId() << "', ";
			query << "	launchtime='" << this->getLaunchtime() << "', ";
			query << "	landtime='" << this->getLandtime() << "', ";
			query << "	nextactiontime='" << this->getNextactiontime() << "', ";
			query << "	status='" << this->getStatus() << "', ";
			query << "	pilots='" << this->getPilots() << "', ";
			query << "	usage_fuel='" << this->usageFuel << "', ";
			query << "	usage_food='" << this->usageFood << "', ";
			query << "	usage_power='" << this->usagePower << "', ";
			query << "	support_usage_fuel='" << this->supportUsageFuel << "', ";
			query << "	support_usage_food='" << this->supportUsageFood << "', ";
			query << "	res_metal='" << this->getResMetal() << "', ";
			query << "	res_crystal='" << this->getResCrystal() << "', ";
			query << "	res_plastic='" << this->getResPlastic() << "', ";
			query << "	res_fuel='" << this->getResFuel() << "', ";
			query << "	res_food='" << this->getResFood() << "', ";
			query << "	res_power='" << this->getResPower() << "', ";
			query << "	res_people='" << this->getResPeople() << "', ";
			query << "	fetch_metal='0', ";
			query << "	fetch_crystal='0', ";
			query << "	fetch_plastic='0', ";
			query << "	fetch_fuel='0', ";
			query << "	fetch_food='0', ";
			query << "	fetch_power='0', ";
			query << "	fetch_people='0' ";
			query << "WHERE ";
			query << "	id='" << this->getId() << "' ";
			query << "LIMIT 1;";
			mysqlpp::Result fsRes = query.store();
			query.reset();
		}
		else {
			query << "DELETE FROM ";
			query << "	fleet ";
			query << "WHERE ";
			query << "	id='" << this->getId() << "' ";
			query << "LIMIT 1;";
			query.store();
			query.reset();
		}
	}

	std::string Fleet::getLogResStart() {
		std::string log = ""
						+ etoa::d2s(this->initResMetal)
						+ ":"
						+ etoa::d2s(this->initResCrystal)
						+ ":"
						+ etoa::d2s(this->initResPlastic)
						+ ":"
						+ etoa::d2s(this->initResFuel)
						+ ":"
						+ etoa::d2s(this->initResFood)
						+ ":"
						+ etoa::d2s(this->initResPeople)
						+ ":"
						+ etoa::d2s(this->initResPower)
						+ ",f,"
						+ etoa::d2s(this->fetchMetal)
						+ ":"
						+ etoa::d2s(this->fetchCrystal)
						+ ":"
						+ etoa::d2s(this->fetchPlastic)
						+ ":"
						+ etoa::d2s(this->fetchFuel)
						+ ":"
						+ etoa::d2s(this->fetchFood)
						+ ":"
						+ etoa::d2s(this->fetchPower)
						+ ":"
						+ etoa::d2s(this->fetchPeople);
		return log;
	}

	std::string Fleet::getLogResEnd() {
		std::string log = ""
						+ etoa::d2s(this->resMetal)
						+ ":"
						+ etoa::d2s(this->resCrystal)
						+ ":"
						+ etoa::d2s(this->resPlastic)
						+ ":"
						+ etoa::d2s(this->resFuel)
						+ ":"
						+ etoa::d2s(this->resFood)
						+ ":"
						+ etoa::d2s(this->resPeople)
						+ ":"
						+ etoa::d2s(this->resPower)
						+ ",f,0:0:0:0:0:0:0";
		return log;
	}

	std::string Fleet::getLogShipsStart() {
		return this->logFleetShipStart;
	}

	std::string Fleet::getLogShipsEnd() {
		if (this->shipsLoaded) {
			std::string log = "";
			std::vector<Object*>::iterator it;
			for (it=objects.begin() ; it < objects.end(); it++) {
				log += etoa::d2s((*it)->getTypeId())
					+ ":"
					+ etoa::d2s((*it)->getCount())
					+ ",";
			}
			return log;
		}
		else
			return "0";
	}
