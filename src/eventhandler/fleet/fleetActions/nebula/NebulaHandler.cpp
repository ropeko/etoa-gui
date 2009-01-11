
#include "NebulaHandler.h"

namespace nebula
{
	void NebulaHandler::update()
	{
	
		/**
		* Fleet action: Collect nebula gas
		*/

		Config &config = Config::instance();
		
		this->actionMessage->addType((int)config.idget("SHIP_MISC_MSG_CAT_ID"));
		
		// Precheck action==possible?
		if (this->f->actionIsAllowed()) {
			// Check if there is a field
			if (this->targetEntity->getCode()=='n' && this->targetEntity->getResSum()>0) {
				this->one = rand() % 101;
				this->two = (int)config.nget("nebula_action",0);
				
				// Ship were destroyed?
				if (this->one  < this->two)	{
					int percent = 100 - rand() % (int)config.nget("nebula_action",1);
					
					this->f->setPercentSurvive(percent/100.0);
				}
				
				if (this->f->actionIsAllowed()) {
					this->sum = 0;
					
					this->nebula = config.nget("nebula_action",2) + (rand() % (int)(this->f->getActionCapacity() - config.nget("nebula_action",2) + 1));
					this->sum +=this->f->addCrystal(this->targetEntity->removeResCrystal(std::min(this->nebula,this->targetEntity->getResCrystal())));
					
					this->actionMessage->addText("Eine Flotte vom Planeten [b]",1);
					this->actionMessage->addText(this->startEntity->getCoords(),1);
					this->actionMessage->addText("[/b]hat den[b]",1);
					this->actionMessage->addText(this->targetEntity->getCoords(),1);
					this->actionMessage->addText("[/b]um [b]");
					this->actionMessage->addText(this->f->getLandtimeString(),1);
					this->actionMessage->addText("[/b]erkundet und Rohstoffe gesammelt.");
					this->actionMessage->addText(this->f->getResCollectedString(),1);
					this->actionMessage->addText(this->f->getDestroyedShipString("\nEinige Schiffe deiner Flotte verirrten sich in einem Interstellarer Gasnebel und konnten nicht mehr gefunden werden.\n\n"));
					
					this->actionMessage->addSubject("Nebelfeld gesammelt");
					
					// Save the collected resources
					this->f->fleetUser->addCollectedNebula(this->sum);
					
				}
				// if there are no nebula collecter in the fleet anymore
				else {
					// Send a message to the user
					this->actionMessage->addText("Eine Flotte vom Planeten [b]",1);
					this->actionMessage->addText(this->startEntity->getCoords(),1);
					this->actionMessage->addText("verirrte sich in einem Interstellarer Gasnebel.");
					
					this->actionMessage->addSubject("Flotte verschollen");
					
					this->actionLog->addText("Action failed: Shot error");
				}
			}
			// If the asteroid field isnt there anymore
			else {
				this->actionMessage->addText("Eine Flotte vom Planeten [b]",1);
				this->actionMessage->addText(this->startEntity->getCoords(),1);
				this->actionMessage->addText("[/b]konnte kein Intergalaktisches Nebelfeld orten und so machte sich die Crew auf den Weg nach Hause.");
				
				this->actionMessage->addSubject("Nebelfeldsammeln gescheitert");
				
				this->actionLog->addText("Action failed: entity error");
			}
		}
		
		// If there isnt any asteroid colecter in the fleet 
		else {
			this->actionMessage->addText("Eine Flotte vom Planeten [b]",1);
			this->actionMessage->addText(this->startEntity->getCoords(),1);
			this->actionMessage->addText("versuchte ein Nebelfeld zu sammeln. Leider war kein Schiff mehr in der Flotte, welches die Aktion ausführen konnte, deshalb schlug der Versuch fehl und die Flotte machte sich auf den Rückweg!");
			
			this->actionMessage->addSubject("Nebelfeldsammeln gescheitert");
			
			this->actionLog->addText("Action failed: Ship error");
		}
		
		this->f->setReturn();
	}
}
