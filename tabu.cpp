#include "tabu.h"
using namespace std;

bool myfunction (collaboration i,collaboration j) {return (i.value>j.value);}

int fighting_experience(vector<int> team_heroes, vector<int> team_villains, 
	vector<collaboration> collab)
{
	unsigned int i, j, k;
	int fighting_exp = 0;
	
	//cout << "\nFIGHTING EXPERIENCE\n";
	for (i = 0; i < team_heroes.size(); i++) {
		for (j = 0; j < team_villains.size(); j++) {
			for (k = 0; k < collab.size(); k++) {
				if(collab[k].c1 == team_heroes[i] && collab[k].c2 == team_villains[j]){
					//cout << team_heroes[i] << "->" << team_villains[j] << "=" << collab[k].value << "\n" ;
					fighting_exp += collab[k].value;
					break;
				}
			}
		}
	}
	
	return fighting_exp;
}

int collaboration_level(vector<int> team_heroes, vector<collaboration> collab)
{
	unsigned int i, j, k;
	int collaboration_lv = 0;
	
	//cout << "\nCOLABORACOES HEROIS\n";
	// calcula a colaboracao enter os herois
	for (i = 0; i < team_heroes.size()-1; i++) {
		for (j = i+1; j < team_heroes.size(); j++) {
			for (k = 0; k < collab.size(); k++) {
				if(collab[k].c1 == team_heroes[i] && collab[k].c2 == team_heroes[j]){
					//cout << team_heroes[i] << "->" << team_heroes[j] << "=" << collab[k].value << "\n" ;
					collaboration_lv += collab[k].value;
					break;
				}
			}
		}
	}	
	
	return collaboration_lv;
}

// verifica se existe este id em um vetor de int
bool in_vector(vector<int> v, int id) {
	unsigned int i;
	for (i = 0; i < v.size(); i++) {
		if(v[i] == id)
			return true;
	}
	return false;
}

bool isHero(int idCharacter) {
	if(idCharacter <= HEROES_SIZE)
			return true;
	return false;
}

bool is_viable_solution(vector<character> heroes, vector<character> villains, 
	vector<int> team_heroes, vector<int> team_villains, int budget)
{
	unsigned int i, idHero, idVillain, teamMaxSize = team_villains.size();
	vector<int> pgHeroes (6);
	vector<int> pgVillains (6);
	
	// verifica se o time de herois tem tamanho <= ao time de viloes
	if(team_heroes.size() > teamMaxSize) {
		return false;
	}
	
	// calcula o powergrid de cada time por habilidade
	for (i = 0; i < 6; i++) {
		pgHeroes[i] = 0;
		pgVillains[i] = 0;
	}
	
	for (i = 0; i < team_heroes.size(); i++) {
		idHero = team_heroes[i] - 1; // ajusta o id
		pgHeroes[0] += heroes[idHero].intelligence;
		pgHeroes[1] += heroes[idHero].strength;
		pgHeroes[2] += heroes[idHero].speed;
		pgHeroes[3] += heroes[idHero].durability;
		pgHeroes[4] += heroes[idHero].energy;
		pgHeroes[5] += heroes[idHero].fighting;
	}
	
	for (i = 0; i < team_villains.size(); i++) {
		idVillain = team_villains[i] - HEROES_SIZE+1; // ajusta o id
		pgVillains[0] += villains[idVillain].intelligence;
		pgVillains[1] += villains[idVillain].strength;
		pgVillains[2] += villains[idVillain].speed;
		pgVillains[3] += villains[idVillain].durability;
		pgVillains[4] += villains[idVillain].energy;
		pgVillains[5] += villains[idVillain].fighting;
	}
	
	// verifica a media do powergrid do time de herois se eh maior ou igual 
	// a media do powergrid do time de viloes para cada habilidade
	for (i = 0; i < 6; i++) {
		if(pgHeroes[i] < pgVillains[i]) return false;
	}
	
	/*
	if(budget){
		// teste
	}
	*/
	return true;
}

/* retorna o id do heroi com maior
 fighting_experience contra os viloes de um determinado time */
unsigned int fighting_experience_max(vector<collaboration> collab, 
	vector<int> team_villains)
{
	int i;
	unsigned int j, k, idHero = 0, fight_exp, fight_exp_max = 0;
	
	for (i = 1; i <= HEROES_SIZE; i++) {
		fight_exp = 0;
		for (j = 0; j < team_villains.size(); j++) {
			for (k = 0; k < collab.size(); k++) {
				if(collab[k].c1 == i && collab[k].c2 == team_villains[j]){
					fight_exp += collab[k].value;
					break;
				}
			}
		}
		// se a fighting_experience desse heroi for maior atualiza
		if(fight_exp_max < fight_exp) {
			fight_exp_max = fight_exp;
			idHero = i;
		}
	}
	return idHero;
}	

/* retorna o id do heroi que melhor maximize a solucao */
unsigned int collab_fighting_max(vector<collaboration> collab, 
	vector<int> team_heroes, vector<int> team_villains, vector<int> tabu_list)
{
	int i;
	unsigned j, k, idHero = 0, value, value_max = 0;
	
	for (i = 1; i <= HEROES_SIZE; i++) {
		// se o heroi nao esta na solucao e nem na lista tabu
		if(!in_vector(team_heroes, i) && !in_vector(tabu_list, i)){
			value = 0;
			// calcula a colaboracao dele com os herois do time
			for (j = 0; j < team_heroes.size(); j++) {
				for (k = 0; k < collab.size(); k++) {
					if(collab[k].c1 == i && collab[k].c2 == team_heroes[j]){
						value += collab[k].value;
						break;
					}
				}
			}
			
			// calcula a fighting_experience desse heroi com os viloes
			for (j = 0; j < team_villains.size(); j++) {
				for (k = 0; k < collab.size(); k++) {
					if(collab[k].c1 == i && collab[k].c2 == team_villains[j]){
						value += collab[k].value;
						break;
					}
				}
			}
			
			// se o valor encontrado para esse heroi for maior, atualiza
			if(value_max < value) {
				value_max = value;
				idHero = i;
			}
		}
	}
	return idHero;
}

void printTeam(vector<int> team){
	unsigned int i;
	cout << "Team\n";
	for (i = 0; i < team.size(); i++) {
		cout << team[i] << " ";
	}
	cout << "\n";
}

// adiciona um heroi na lista tabu, FIFO
void add_tabu_list(vector<int> &tabu_list, vector<int> team_villains, 
	int idHero){
	//if(tabu_list.size() == team_villains.size()*3){
	if(tabu_list.size() == TABU_LIST_MAX){
		// retira o primeiro da lista
		tabu_list.erase(tabu_list.begin());	
	}
	tabu_list.push_back(idHero);
}

vector<int> initial_solution(vector<character> heroes, vector<character> villains, 
	vector<collaboration> collab, vector<int> team_villains, int budget)
{
	unsigned int idHero, idRm = 1, cont = 0, teamMaxSize = team_villains.size();
	vector<int> team_heroes, tabu_list;
	bool is_viavel = false;
	
	// busca heroi com maior fighting_experience contra os viloes inclui na solucao
	idHero = fighting_experience_max(collab, team_villains);
	team_heroes.push_back(idHero);
	
	do {
		if(team_heroes.size() == teamMaxSize && 
			is_viable_solution(heroes, villains, team_heroes, team_villains, budget)){
			is_viavel = true;
		}
		else{
			if(cont == TABU_LIST_MAX){
				idRm++;
				cont = 0;
			}
		
			// se o numero de herois superou o numero de viloes
			if(team_heroes.size() >= teamMaxSize){
				// retire um heroi e acrescenta na lista tabu
				/*idRand = rand() % team_heroes.size();
				idHero = team_heroes[idRand];
				team_heroes.erase(team_heroes.begin()+idRand);*/
				
				idHero = team_heroes[team_heroes.size()-idRm];
				team_heroes.erase(team_heroes.end()-idRm);	
				add_tabu_list(tabu_list, team_villains, idHero);
				
				idRm = 1;
				cont++;
				//cout << "Removeu\n";
				//printTeam(team_heroes);
			}
			
			// busca o heroi que tem maior collaboration_level + fighting_experience
			// e que nao esteja na lista tabu, acrescenta na solucao
			idHero = collab_fighting_max(collab, team_heroes, team_villains, tabu_list);
			team_heroes.push_back(idHero);
			
			//printTeam(team_heroes);
		}
	} while(!is_viavel);
	
	return team_heroes;
}

/* busca de uma boa solucao (mesmo que inviavel) 
partindo da solucao retornada pelo algoritmo de construcao inicial (que eh a
melhor mas nao e viavel) verifico se ela eh tem custo > budget se sim, troca
o melhor heroi por um heroi pior (seguindo algum criterio) se nao, troca o pior
heroi por um heroi melhor. Faça isso por um numero de iteracoes fixo.
*/

/* busca local por uma solucao viavel
partindo da solucao do algoritmo que encontra uma boa solucao mas nao
necessariamente viavem, executa uma busca local, ou seja, troca heroi pelo
seus vizinhos em termos de custo, a fim de encontrar a melhor solucao que 
seja viavel
*/