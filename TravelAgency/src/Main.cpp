#include "TravelAgency.h"

int main(){
	TravelAgency* travelAgency = new TravelAgency();
	travelAgency->chooseGraph();
	travelAgency->processGraph();
	travelAgency->mainMenu();
}
