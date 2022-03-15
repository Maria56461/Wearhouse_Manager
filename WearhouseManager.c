#include <stdio.h>
#include "WearhouseManager.h"
#define MAX_VALUE 20

Package *create_package(long priority, const char* destination){

	Package* new_package = (Package*) malloc(sizeof(Package));
	
	if (priority < 0)
		return NULL;

	if (new_package == NULL) {
		printf("Alocarea a esuat!\n");
		exit(1);
	}

	if (destination != NULL) {
		new_package->destination = (char*) malloc(MAX_VALUE * sizeof(char));
		if (new_package->destination == NULL) {
			printf("Alocarea a esuat!\n");
			exit(1);
		}
		strcpy(new_package->destination, destination);
	} 
	else if (destination == NULL)
		new_package->destination = NULL;
	
	new_package->priority = priority;
	
return new_package;
}

void destroy_package(Package* package) {

 	if (package == NULL)
		return;
	
	if (package->destination != NULL)
		free(package->destination);
	free(package);
	
}

Manifest* create_manifest_node(){
	
	Manifest* first;
	first = (Manifest*) malloc(sizeof(Manifest));

	if (first == NULL) {
		printf("Alocarea a esuat!\n");
		exit(1);
	}

	first->package = NULL;
	first->next = NULL;
	first->prev = NULL; 

return first;
}

void destroy_manifest_node(Manifest* manifest_node){

	if (manifest_node == NULL)
		return;

	if (manifest_node->package != NULL)
		destroy_package(manifest_node->package);
	free(manifest_node);
}

Wearhouse* create_wearhouse(long capacity){

	Wearhouse* wearhouse;

	if (capacity <= 0)
		return NULL;
	else {
		wearhouse = (Wearhouse*) malloc(sizeof(Wearhouse));
		if (wearhouse == NULL) {
			printf("Alocarea a esuat!\n");
			exit(1);
		}
		wearhouse->packages = (Package **) malloc(capacity * sizeof(Package*));
		if (wearhouse->packages == NULL) {
			printf("Alocarea a esuat!\n");
			exit(1);
		}
		wearhouse->capacity = capacity;
		wearhouse->size = 0;
	}

return wearhouse;
}

Wearhouse *open_wearhouse(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Wearhouse *w = NULL;


	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	if((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		free(line);
		line = NULL;
		len = 0;
	}

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package *p = create_package(priority, token);
		w->packages[w->size++] = p;

		free(line);
		line = NULL;
		len = 0;
	}

	free(line);


	fclose(fp);
	return w;

	file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse *w){
	
	if (w == NULL)
		return 0;

	if (w->size > w->capacity)
		return 0;

	if ((w->size == 0) && (w->capacity >= 0))
		return 1;

return 0;
}

long wearhouse_is_full(Wearhouse *w){

	if (w == NULL)
		return 0;
	
	if ((w->size == w->capacity) && (w->capacity > 0))
		return 1;

return 0;
}

long wearhouse_max_package_priority(Wearhouse *w){
	
	long a = w->packages[0]->priority;
	int i;

	for (i = 1; i < w->size; i++) {
	//parcurg array-ul de pachete
		if (w->packages[i]->priority > a) {
			a = w->packages[i]->priority;
		}
	}
	
return a;
}

long wearhouse_min_package_priority(Wearhouse *w){
	
	int i;
	long a = w->packages[0]->priority;

	for (i = 1; i < w->size; i++) {
	//parcurg array-ul de pachete
		if (w->packages[i]->priority < a)
			a = w->packages[i]->priority;
	}
	
return a;
}


void wearhouse_print_packages_info(Wearhouse *w){
	for(long i = 0; i < w->size; i++){
		printf("P: %ld %s\n",
				w->packages[i]->priority,
				w->packages[i]->destination);
	}
	printf("\n");
}

void destroy_wearhouse(Wearhouse* wearhouse){

	if (wearhouse == NULL)
		return;
	
	for (int i = 0; i < wearhouse->size; i++)
		if (wearhouse->packages[i] != NULL)
			free(wearhouse->packages[i]);

	free(wearhouse->packages);
	free(wearhouse);
	wearhouse = NULL;

}

Robot* create_robot(long capacity){

	if (capacity < 0)
		return NULL;

	Robot* robot;	
	robot = (Robot*) malloc(sizeof(Robot));

	if (robot == NULL) {
			printf("Alocarea a esuat!\n");
			exit(1);
		}

	robot->manifest = NULL;
	robot->capacity = capacity;
	robot->size = 0;
	robot->next = NULL;
	
return robot;
}

int robot_is_full(Robot* robot){
	
	if ((robot->size == robot->capacity) && (robot->capacity >= 0))
		return 1;

return 0;
}

int robot_is_empty(Robot* robot){

	if ((robot->size == 0) && (robot->capacity >= 0))
		return 1;

return 0;
}

Package* robot_get_wearhouse_priority_package(Wearhouse *w, long priority){
	
	int i;
 
	if (priority < 0)
		return NULL;
	else
		if (w->size > 0)
			for (i = 0; i < w->size; i++)
				if (w->packages[i]->priority == priority) 
					return w->packages[i]; 

return NULL;
}

void robot_remove_wearhouse_package(Wearhouse *w, Package* package){

	int i, j, k = 0;

	for (i = 0; i < w->size; i++) 
		if (w->packages[i] == package) {
		//parcurg vectorul de pachete din depozit
		//pana cand gasesc pachetul dorit
			k = i;
			//retin indicele sau
			break;
		}

	if (k <= w->size) {
		for (j = k; j < (w->size - 1); j++) 
			w->packages[j] = w->packages[j+1];
			//mutare cu o pozitie mai la stanga
		w->size = w->size - 1;	
	}
	
}

void robot_load_one_package(Robot* robot, Package* package){

	Manifest *new_package, *current1;
	new_package = create_manifest_node();
	new_package->package = package;
	new_package->prev = NULL;
	new_package->next = NULL;

	if (robot->capacity <= robot->size)
	//lista plina
		return ;

	if (package->priority < 0)
		return ;

	if (robot->size == 0) {
	//lista goala
		robot->manifest = new_package;
		robot->size = robot->size + 1; }
	else {
	//lista nu e plina, dar exista elemente in lista
		current1 = robot->manifest; 
		while ((current1->package->priority > package->priority) 
					&& (current1->next != NULL))
		//gasesc prioritate mai mica sau egala ori ajung la ultimul nod al listei
			current1 = current1->next;
		if ((current1->next == NULL) && 
				(((current1->package->priority == package->priority) &&
				(strcmp(current1->package->destination, package->destination) <= 0)) ||
				(current1->package->priority > package->priority))) {
		//inserez dupa current1, la finalul listei
			current1->next = new_package;
			new_package->prev = current1;
			robot->size = robot->size + 1; }
		else if (((current1->next == NULL) && 
						(((current1->package->priority == package->priority) &&
						(strcmp(current1->package->destination, package->destination) > 0)
						&& (current1 != robot->manifest)) || 
						(current1->package->priority < package->priority))) || 
						(((current1->next != NULL) && (robot->manifest != current1)) && 
						((current1->package->priority < package->priority) || 
						((current1->package->priority == package->priority) &&
						(strcmp(current1->package->destination, package->destination) > 0))))) {
		//inserez la mijlocul listei, imediat inapoia lui current	 
			new_package->next = current1;
			new_package->prev = current1->prev;
			current1->prev->next = new_package;
			current1->prev = new_package; 
			robot->size = robot->size + 1; }
		else if ((robot->manifest == current1) && 
						((current1->package->priority < package->priority) ||
						(((current1->package->priority == package->priority) &&
						(strcmp(current1->package->destination, package->destination) >= 0))))) {
		//inserare la inceputul listei, imediat inapoia lui current
			new_package->next = current1;
			current1->prev = new_package;
			robot->manifest = new_package;
			robot->size = robot->size + 1; }
		else if ((robot->manifest == current1) && (current1->next == NULL) &&
						(current1->package->priority == package->priority) && 
						(strcmp(current1->package->destination, package->destination) > 0)) {
			new_package->next = current1;
			current1->prev = new_package;
			robot->manifest = new_package;
			robot->size = robot->size + 1; 
		}
		else if ((current1->next != NULL) && 
						(current1->package->priority == package->priority) &&
						(strcmp(current1->package->destination, package->destination) <= 0)) {
		//inaintez prin lista pana gasesc destinatie mai mare sau prioritate mai mica
		//sau ajung la finalul listei
			while ((strcmp(current1->package->destination, package->destination) < 0) 
						&& (current1->package->priority == package->priority) 
						&& (current1->next != NULL)) 
				current1 = current1->next;
			if ((current1->next == NULL) && 
				 (strcmp(current1->package->destination, package->destination) < 0) &&
				 (current1->package->priority == package->priority)) { 
			//inserez la final de lista, dupa current1
				new_package->prev = current1;
				current1->next = new_package;
				robot->size = robot->size + 1; }
			else {
			//inserez fix inapoia lui current1, la mijlocul listei
				new_package->prev = current1->prev;
				new_package->next = current1;
				current1->prev->next = new_package;
				current1->prev = new_package; 
				robot->size = robot->size + 1; } 
		}
	}

}

long robot_load_packages(Wearhouse* wearhouse, Robot* robot){

	Package* package;
	long nr_pachete = 0, max_priority;

	if ((robot->capacity < 0) || (robot->size < 0) || 
			(robot->size >= robot->capacity) || (wearhouse->capacity < 0) ||
			(wearhouse->size < 0) || (wearhouse->size > wearhouse->capacity))
	return 0;

	while ((robot->size < robot->capacity) && (wearhouse->size > 0)) {
		max_priority = wearhouse_max_package_priority(wearhouse);
		package = robot_get_wearhouse_priority_package(wearhouse, max_priority);				
		robot_load_one_package(robot, package);
		robot_remove_wearhouse_package(wearhouse, package);
		nr_pachete = nr_pachete + 1;	
	}

return nr_pachete;
}

Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination){
	
	Manifest* current1 = robot->manifest, *current2 = robot->manifest;
	long priorities[500] = {0, 0, 0};
	int i = 0;

	if (robot->manifest == NULL)
		return NULL;

	while (current1 != NULL) {
	//parcurg lista de pachete ale robotului
		if (strcmp(current1->package->destination, destination) == 0) {
		//cand gasesc pachet cu destinatia respectiva
			priorities[i] = current1->package->priority;
			//ii pun prioritatea intr-un vector	
			i = i + 1; 
		}
		current1 = current1->next;
	}
	
	long init = priorities[0];

	for (int j = 0; j < 500; j++)
		if (priorities[j] > init)
			init = priorities[j];
	// am aflat prioritatea maxima a pachetelor cu destinatia particulara

	while (current2 != NULL) {
		if ((current2->package->priority == init) && 
				(strcmp(current2->package->destination, destination) == 0))
			break;
		current2 = current2->next;
	} 
	//current2 - primul pachet cu destinatia particulara si prioritatea maxima

	if ((current2 == robot->manifest) && 
		 (strcmp(current2->package->destination, destination) != 0))
	//nu exista pachet cu destinatia respectiva
		return NULL;
 
return current2->package;
}

void destroy_robot(Robot* robot){

	if (robot == NULL)
		return;

	Manifest* current = robot->manifest;

	while (current != NULL) {
		Manifest* copy = current->next; 
		destroy_manifest_node(current);
		current = copy;
	} //dezaloc itinerariu

	robot->manifest = NULL;
	
	free(robot);
	//dezaloc robot

}

void robot_unload_packages(Truck* truck, Robot* robot){ 
	
	Manifest* current = robot->manifest, *copie; 

	if ((robot->manifest == NULL) || (robot->size == 0))
		return ;
	
	if (truck->capacity == truck->size) 
		return ;

	while ((current != NULL) && (robot->size != 0)) {
	//parcurg lista de pachete ale robotului
		copie = current->next;
		if (strcmp(current->package->destination, truck->destination) == 0) {
		//caut pachet cu destinatia tirului 
			if (truck->size == truck->capacity) {
				return ;                 
			} 
			//daca tirul e plin, nu il pot transfera                                 
			if ((current == robot->manifest) && (current->next == NULL)) { 
			//daca pachetul e singurul din lista robotului
				robot->manifest = NULL;  
				//il elimin din itinerariul robotului               
				if (truck->manifest == NULL) {
					truck->manifest = current;
					truck->size = truck->size + 1;
				} 
				//il adaug in tir; consider cazul cand tirul este gol
				else {
					current->next = truck->manifest;
					current->prev = NULL;
					truck->manifest = current;
					current->next->prev = current;
					truck->size = truck->size + 1;   
				} 
				robot->size = robot->size - 1;
				//size-ul robotului scade!! 		
			} else  
			if (current == robot->manifest) {
			//daca pachetul e primul si nu singurul din lista robotului
				robot->manifest = current->next;
				current->next->prev = NULL;
				if (truck->manifest == NULL) {
					current->next = NULL;
					current->prev = NULL;
					truck->manifest = current;
					truck->size++;
				} 
				else {
					current->next = truck->manifest;
					current->prev = NULL;
					truck->manifest = current;
					current->next->prev = current;
					truck->size++;   
				}  
				robot->size--;				
			} else 
			if (current->next == NULL) {
			//daca pachetul e ultimul din lista robotului
				current->prev->next = NULL;
				if (truck->manifest == NULL) {
					truck->manifest = current;
					current->prev = NULL;
					truck->size++;
				} else {
					current->prev = NULL;
					current->next = truck->manifest;
					truck->manifest = current;
					current->next->prev = current;
					truck->size++;   
				}  
				robot->size--;
			} 
			else {
			//daca pachetul e in mijlocul itinerariului robotului
				current->next->prev = current->prev;
				current->prev->next = current->next;
				if (truck->manifest == NULL) {
					truck->manifest = current;
					current->prev = NULL;
					current->next = NULL;
					truck->size++;
				} 
				else {
					current->next = truck->manifest;
					truck->manifest = current;
					current->next->prev = current;
					current->prev = NULL;
					truck->size++;   
				}  
				robot->size--;
			} 
		} 
		current = copie;			
	} 

return ;
}

// Attach to specific truck
int robot_attach_find_truck(Robot* robot, Parkinglot *parkinglot){
	int found_truck = 0;
	long size = 0;
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	Manifest* m_iterator = robot->manifest;


	while(m_iterator != NULL){
		while(arrived_iterator != parkinglot->arrived_trucks){
			size  = truck_destination_robots_unloading_size(arrived_iterator);
			if(strncmp(m_iterator->package->destination, arrived_iterator->destination, MAX_DESTINATION_NAME_LEN) == 0 &&
					size < (arrived_iterator->capacity-arrived_iterator->size)){
				found_truck = 1;
				break;
			}

			arrived_iterator = arrived_iterator->next;
		}

		if(found_truck)
			break;
		m_iterator = m_iterator->next;
	}

	if(found_truck == 0)
		return 0;


	Robot* prevr_iterator = NULL;
	Robot* r_iterator = arrived_iterator->unloading_robots;
	while(r_iterator != NULL){
		Package *pkg = robot_get_destination_highest_priority_package(r_iterator, m_iterator->package->destination);
		if(m_iterator->package->priority >= pkg->priority)
			break;
		prevr_iterator = r_iterator;
		r_iterator = r_iterator->next;
	}

	robot->next = r_iterator;
	if(prevr_iterator == NULL)
		arrived_iterator->unloading_robots = robot;
	else
		prevr_iterator->next = robot;

	return 1;
}

void robot_print_manifest_info(Robot* robot){
	Manifest *iterator = robot->manifest;
	while(iterator != NULL){
		printf(" R->P: %s %ld\n", iterator->package->destination, iterator->package->priority);
		iterator = iterator->next;
	}

	printf("\n");
}

Truck* create_truck(const char* destination, long capacity, long transit_time, long departure_time){

	Truck* truck;
	truck = malloc(sizeof(Truck));

	if ((capacity < 0) || (transit_time < 0) || (departure_time < 0))
		return NULL;

	if (truck == NULL) {
		printf("Alocarea a esuat!");
		exit(1);
	} 

	truck->manifest = NULL;
	truck->unloading_robots = NULL;

	if (destination != NULL) {
		truck->destination = (char*) malloc(MAX_VALUE * sizeof(char));
		if (truck->destination == NULL) {
			printf("Alocarea a esuat!");
			return NULL;
		}
		strcpy(truck->destination, destination);
	} 
	else if (destination == NULL)
		truck->destination = NULL;

	truck->size = 0;
	truck->capacity = capacity;
	truck->in_transit_time = 0;
	truck->departure_time = departure_time;
 	truck->transit_end_time	= transit_time;
	truck->next = NULL;
	
return truck;
}

int truck_is_full(Truck *truck){

	if ((truck->size == truck->capacity) && (truck->capacity >= 0))
		return 1;

return 0;
}

int truck_is_empty(Truck *truck){

	if ((truck->size == 0) && (truck->capacity >= 0))
		return 1;

return 0;
}

long truck_destination_robots_unloading_size(Truck* truck){
	
	if (truck->unloading_robots == NULL)
		return 0;

	long s = 0, p = 0;
	Robot* robots_list = truck->unloading_robots;
	
	while (robots_list != NULL) {
		Manifest* packages_list = robots_list->manifest;
		while (packages_list->next != NULL) {
			if (strcmp(packages_list->package->destination, truck->destination) == 0) 
				p++;
			packages_list = packages_list->next;
		}
		if (p != 0)
			s = s + robots_list->size;
		p = 0;
		robots_list = robots_list->next;
	}

return s;
}

void truck_print_info(Truck* truck){
	printf("T: %s %ld %ld %ld %ld %ld\n", truck->destination, truck->size, truck->capacity,
			truck->in_transit_time, truck->transit_end_time, truck->departure_time);

	Manifest* m_iterator = truck->manifest;
	while(m_iterator != NULL){
		printf(" T->P: %s %ld\n", m_iterator->package->destination, m_iterator->package->priority);
		m_iterator = m_iterator->next;
	}

	Robot* r_iterator = truck->unloading_robots;
	while(r_iterator != NULL){
		printf(" T->R: %ld %ld\n", r_iterator->size, r_iterator->capacity);
		robot_print_manifest_info(r_iterator);
		r_iterator = r_iterator->next;
	}
}

void destroy_truck(Truck* truck){

	Manifest* current = truck->manifest;
	Robot* curent = truck->unloading_robots;

	if (truck == NULL)
		return ;
	
	if (truck->manifest != NULL) {
		while (current != NULL) {
			Manifest* copy = current->next;
			destroy_manifest_node(current);
			current = copy;
		}
		truck->manifest = NULL;
	}

	if (truck->unloading_robots != NULL) {
		while (curent != NULL) {
			Robot* aux = curent->next;
			destroy_robot(curent);
			curent = aux;
		}
		truck->unloading_robots = NULL;
	}

	if (truck->destination != NULL)
		free(truck->destination);

	free(truck);

}

Parkinglot* create_parkinglot(void){
	
	Parkinglot* parkinglot;
	parkinglot = (Parkinglot*) malloc(sizeof(Parkinglot));

	if (parkinglot != NULL) {
		parkinglot->arrived_trucks = create_truck(NULL, 0, 0, 0);
		parkinglot->departed_trucks = create_truck(NULL, 0, 0, 0);
		parkinglot->pending_robots = create_robot(0);
		parkinglot->standby_robots = create_robot(0);
		parkinglot->arrived_trucks->next = parkinglot->arrived_trucks;
		parkinglot->departed_trucks->next = parkinglot->departed_trucks;
		parkinglot->pending_robots->next = parkinglot->pending_robots;
		parkinglot->standby_robots->next = parkinglot->standby_robots;
	}

return parkinglot;
}

Parkinglot* open_parckinglot(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Parkinglot *parkinglot = create_parkinglot();

	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		// destination, capacitym transit_time, departure_time, arrived
		if(token[0] == 'T'){
			token = strtok(NULL, ",\n ");
			char *destination = token;

			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			token = strtok(NULL, ",\n ");
			long transit_time = atol(token);

			token = strtok(NULL, ",\n ");
			long departure_time = atol(token);

			token = strtok(NULL, ",\n ");
			int arrived = atoi(token);

			Truck *truck = create_truck(destination, capacity, transit_time, departure_time);

			if(arrived)
				truck_arrived(parkinglot, truck);
			else
				truck_departed(parkinglot, truck);

		}else if(token[0] == 'R'){
			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			Robot *robot = create_robot(capacity);
			parkinglot_add_robot(parkinglot, robot);

		}

		free(line);
		line = NULL;
		len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

	file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot *robot){

	if (robot == NULL)
		return ;

	Robot* standby_list = parkinglot->standby_robots, *prev;
	Robot* pending_list = parkinglot->pending_robots, *prev3;
	
	if ((robot->size == 0) && (robot->capacity >= 0)) {
	//il adaug in lista de standby_robots
		do {
			prev = standby_list;
			standby_list = standby_list->next; 
		}
		while ((standby_list != parkinglot->standby_robots) && 
					(standby_list->capacity > robot->capacity));
		//parcurg lista de standby_robots pana gasesc o capaciatate mai mica 
		//ori ajung la finalul listei
			robot->next = standby_list;
			prev->next = robot;
	}
	else if ((robot->size > 0) && (robot->capacity >=0)){
		do {
			prev3 = pending_list;
			pending_list = pending_list->next;
		}
		while ((pending_list != parkinglot->pending_robots) && 
					(pending_list->size > robot->size)) ;
			//parcurg pana cand gasesc size mai mic sau egal
			//ori ajung la finalul listei
			robot->next = pending_list;
			prev3->next = robot;
	}
				
}

void parkinglot_remove_robot(Parkinglot *parkinglot, Robot* robot){

	if (robot == NULL)
		return ;
	
	if ((parkinglot->pending_robots == parkinglot->pending_robots->next) 
		 && (robot->size > 0))
		return ;

	if ((parkinglot->standby_robots == parkinglot->standby_robots->next) 
		 && (robot->size == 0))
		return ;
	
	if ((robot->size == 0) && (robot->capacity >= 0)) {
	//il elimin din lista de standby cu cel putin santinela si un element
		Robot* standby_list = parkinglot->standby_robots;
		Robot* prev;
		do {
			prev = standby_list;
			standby_list = standby_list->next; }
		while ((standby_list != parkinglot->standby_robots) && 
					(standby_list != robot));
		if (standby_list == parkinglot->standby_robots)
		//nu am gasit robotul	
			return ;	
		prev->next = standby_list->next;
	}
	else if (robot->size > 0) {
		Robot* pending_list = parkinglot->pending_robots;
		Robot* prev3;
		do {
			prev3 = pending_list;
			pending_list = pending_list->next;
		}
		while ((pending_list != parkinglot->pending_robots) && 
					(pending_list != robot)) ;
		prev3->next = pending_list->next;
	}
		
}

int parckinglot_are_robots_peding(Parkinglot* parkinglot){
	
	if (parkinglot->pending_robots->next != parkinglot->pending_robots)
		return 1;

return 0;
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot){

	Truck* current = parkinglot->arrived_trucks;

	do {
		current = current->next;
	}
	while ((current != parkinglot->arrived_trucks) && (current->size == 0));

	if (current == parkinglot->arrived_trucks)
		return 1;

return 0;
}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot){

	if (parkinglot->departed_trucks->next != parkinglot->departed_trucks)
		return 1;

return 0;
}

void destroy_parkinglot(Parkinglot* parkinglot){
	
	if (parkinglot->arrived_trucks != NULL) {
		Truck* current = parkinglot->arrived_trucks->next;
		while (current != parkinglot->arrived_trucks) {
			Truck* copy = current->next;	
			destroy_truck(parkinglot->arrived_trucks);
			current = copy;
		}
		parkinglot->arrived_trucks = NULL;
	}
		
	if (parkinglot->departed_trucks != NULL) {
		Truck* current2 = parkinglot->departed_trucks->next;
		while (current2 != parkinglot->departed_trucks) {
			Truck* copy2 = current2->next;	
			destroy_truck(parkinglot->departed_trucks);
			current2 = copy2;
		}
		parkinglot->departed_trucks = NULL; 
	}

	if (parkinglot->pending_robots != NULL) {
		Robot* current3 = parkinglot->pending_robots->next;
		while (current3 != parkinglot->pending_robots) {
			Robot* copy3 = current3->next;	
			destroy_robot(parkinglot->pending_robots);
			current3 = copy3;
		}
		parkinglot->pending_robots = NULL; 
	}

	if (parkinglot->standby_robots != NULL) {
		Robot* current4 = parkinglot->standby_robots->next;
		while (current4 != parkinglot->standby_robots) {
			Robot* copy4 = current4->next;	
			destroy_robot(parkinglot->standby_robots);
			current4 = copy4;
		}
		parkinglot->standby_robots = NULL; 
	}

	if (parkinglot != NULL)
		free(parkinglot);

}

void parkinglot_print_arrived_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->arrived_trucks->next;
	while(iterator != parkinglot->arrived_trucks){

		truck_print_info(iterator);
		iterator = iterator->next;
	}

	printf("\n");

}

void parkinglot_print_departed_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->departed_trucks->next;
	while(iterator != parkinglot->departed_trucks){
		truck_print_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_pending_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->pending_robots->next;
	while(iterator != parkinglot->pending_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->standby_robots->next;
	while(iterator != parkinglot->standby_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void truck_departed(Parkinglot *parkinglot, Truck* truck){

	Truck* arrived_list = parkinglot->arrived_trucks, *prev;
	Truck* departed_list = parkinglot->departed_trucks, *prev2;

	do {
			prev = arrived_list;
			arrived_list = arrived_list->next;
		}
		while ((arrived_list != parkinglot->arrived_trucks) &&
					(arrived_list != truck));
	//parcurg lista de tiruri sosite; ori ajung la finalul listei, ori gasesc tirul

	if (arrived_list != parkinglot->arrived_trucks) 
	//daca am gasit tirul, il elimin 
		prev->next = arrived_list->next;
		
	do {
			prev2 = departed_list;
			departed_list = departed_list->next;
		}
		while ((departed_list != parkinglot->departed_trucks) &&
					(departed_list->departure_time <= truck->departure_time));
	//parcurg lista de tiruri plecate; ori ajung la finalul listei, ori gasesc 
	//departure_time mai mare 

	truck->next = departed_list;
	prev2->next = truck;

}

void truck_arrived(Parkinglot *parkinglot, Truck* truck){

	if (parkinglot == NULL || truck == NULL) 
		return ;

	Truck* departed_list = parkinglot->departed_trucks, *prev3;
	Truck* arrived_list = parkinglot->arrived_trucks, *prev;
	Manifest *pachete = truck->manifest;

	do {
			prev3 = departed_list;
			departed_list = departed_list->next;
	}
	while ((departed_list != parkinglot->departed_trucks) &&
				(departed_list != truck));
	
	if (departed_list == truck) 
	//am gasit tirul, deci il elimin
		prev3->next = departed_list->next;

	do {
		prev = arrived_list;
		arrived_list = arrived_list->next;
	} 
	while ((arrived_list != parkinglot->arrived_trucks) &&
				(strcmp(arrived_list->destination, truck->destination) <= 0) &&
				((arrived_list->departure_time < truck->departure_time) ||
				(strcmp(arrived_list->destination, truck->destination) != 0)));
		//parcurg pana gasesc aceeasi dest cu departure_time mai mare sau egal 
		//ori o destinatie mai mare strict, ori ajung la finalul listei
		truck->next = arrived_list;
		prev->next = truck;

	while (pachete != NULL) {
	//tirul sosit nu mai are pachete
		Manifest* copie = pachete;
		pachete = pachete->next;
		destroy_manifest_node(copie);
	}

	truck->in_transit_time = 0;
	truck->size = 0;
	truck->manifest = NULL;
	
}

void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck){
	
	Robot* unloading_list = truck->unloading_robots, *prev;

	if ((truck == NULL) || (parkinglot == NULL))
		return;

	while (unloading_list != NULL) {
	//parcurg lista de roboti, retin mereu predecesorul
		prev = unloading_list;
		unloading_list = unloading_list->next;
		parkinglot_add_robot(parkinglot, prev);
	}
	
	truck->unloading_robots = NULL;

}

void truck_update_depatures(Parkinglot* parkinglot, long day_hour){

	Truck* arrived_list = parkinglot->arrived_trucks, *prev;

	if (parkinglot == NULL)
		return;

	do {
	//parcurg lista de tiruri sosite
		prev = arrived_list;
		arrived_list = arrived_list->next;
		if (prev->departure_time == day_hour) {
			//daca este timpul de plecare
			truck_transfer_unloading_robots(parkinglot, prev);
			truck_departed(parkinglot, prev);
		}
	}
	while (arrived_list != parkinglot->arrived_trucks);

} 

void truck_update_transit_times(Parkinglot* parkinglot){

	Truck* departed_list = parkinglot->departed_trucks, *prev;
	
	do {
		prev = departed_list;
		departed_list = departed_list->next;
		prev->in_transit_time = prev->in_transit_time + 1;
		if (prev->in_transit_time == prev->transit_end_time) {
		//daca s-a terminat timpul de plecare
			prev->in_transit_time = 0;
			truck_arrived(parkinglot, prev);
			//adaug tirul in lista celor sosite 
		}
	} while (departed_list != parkinglot->departed_trucks); 

}

void robot_swarm_collect(Wearhouse *wearhouse, Parkinglot *parkinglot){
	Robot *head_robot = parkinglot->standby_robots;
	Robot *current_robot = parkinglot->standby_robots->next;
	while(current_robot != parkinglot->standby_robots){

		// Load packages from wearhouse if possible
		if(!robot_load_packages(wearhouse, current_robot)){
			break;
		}

		// Remove robot from standby list
		Robot *aux = current_robot;
		head_robot->next = current_robot->next;
		current_robot = current_robot->next;

		// Add robot to the
		parkinglot_add_robot(parkinglot, aux);
	}
}


void robot_swarm_assign_to_trucks(Parkinglot *parkinglot){

	Robot *current_robot = parkinglot->pending_robots->next;

	while(current_robot != parkinglot->pending_robots){
		Robot* aux = current_robot;
		current_robot = current_robot->next;
		parkinglot_remove_robot(parkinglot, aux);
		int attach_succeded = robot_attach_find_truck(aux, parkinglot);
		if(!attach_succeded)
			parkinglot_add_robot(parkinglot, aux);
	}
}

void robot_swarm_deposit(Parkinglot* parkinglot){
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	while(arrived_iterator != parkinglot->arrived_trucks){
		Robot *current_robot = arrived_iterator->unloading_robots;
		while(current_robot != NULL){
			robot_unload_packages(arrived_iterator, current_robot);
			Robot *aux = current_robot;
			current_robot = current_robot->next;
			arrived_iterator->unloading_robots = current_robot;
			parkinglot_add_robot(parkinglot, aux);
		}
		arrived_iterator = arrived_iterator->next;
	}
}

