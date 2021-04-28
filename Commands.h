# include "Country.h"

/* Implemention of the vaccinateStatusBloom command.
 * vaccinationStatusBloom: Takes an id and a virus. Checks at the virus bloomfilter if the id has been inserted and prints the suitable message.
 * 
 * Vlist: Virus list. (See Virus.h)
 * Id: Citizens Id that we want to see hers/his vaccination status. In string form
 * VirusName: Name of the virus that we want to check.
 * 
*/
void VaccinateStatusBloom(Virus * Vlist, char * Id, char * VirusName);

/* Implemention of the vaccinateStatus command.
 * vaccinationStatus: Takes an id and a virus. Checks at the virus bloomfilter if the id has been inserted and prints the suitable message.
 * If you don't give any virusName, the program prints the vaccinationStatus of the citizen for every virus that is kept to the program.
 * 
 * Vlist: Virus list. (See Virus.h)
 * Id: Citizens Id that we want to see hers/his vaccination status. In string form
 * VirusName: Name of the virus that we want to check.
 * 
*/
void VaccinateStatus(Virus * Vlist, char * Id, char * VirusName);

/* Implemention of the list-nonVaccinated-Persons command.
 * list-nonVaccinated-Persons: Takes a virus and print all the citizens that hasn't been vaccinated.
 * 
 * Vlist: Virus list. (See Virus.h)
 * VName: Name of the virus that we want to check.
 * 
*/
void ListNonVaccinated(Virus * Vlist, char * VName);

/* Implemention of the insertCitizenRecord command.
 * insertCitizenRecord: Give the information of a citizen and the program adds her/him to the "data base".
 * 
 * Vlist: Virus list. (See Virus.h)
 * CList: Country list. (See Country.h)
 * Array: Array of all the citizen informations.
 * 
*/
void InsertCitizenRecord(Virus * Vlist, Country * CList, char ** Array);

/* Implemention of the vaccinateNow command.
 * vaccinatedNow: Give the information of a citizen and the program deletes him/her from the nonvaccinated and put him/her to the vaccinated.
 * If the citizen is already vaccinated, the program prints an appropriate message.
 * 
 * Vlist: Virus list. (See Virus.h)
 * CList: Country list. (See Country.h)
 * Array: Array of all the citizen informations.
 * 
*/
void VaccinateNow(Virus * Vlist, Country * CList, char ** Array);

/* Implemention of the populationStatus and populationStatusByAge commands.
 * populationStatus: takes a country, a virus and 2 dates and prints the statistics for the vaccinations that have taken place between the 2 dates for the country.
 * populationStatusByAge: takes a country, a virus and 2 dates and prints the statistics for the vaccinations that have taken place between the 2 dates for the country per age group.
 * both commands can be called without the country field. Then both will print the things that have been said but for every country inside the "data base".
 * 
 * Vlist: Virus list. (See Virus.h).
 * CList: Country list. (See Country.h).
 * Array: Array of all the citizen informations.
 * Age: boolean for the declaration between the populationStatus and populationStatusByAge.
 * 
*/
void populationStatus(Virus * VList, Country * CList, char ** Array, bool Age);

/* It is used by the populationStatus function for the calculation of the vaccinated citizens per country.
 * It returns an array with the number of citizens that have been vaccinated for each age group.
 * 
 * Vlist: Virus list. (See Virus.h).
 * Country: Name of country.
 * Age: boolean for the declaration between the populationStatus and populationStatusByAge.
 * Array: int array that is going to keep the number of vaccinated citizens per age group.
 * Date1: from date.
 * Date2: to date.
 * 
*/
int * CalculateVaccinated(Virus * VList, char * Country, bool Age, int * Array, char * Date1, char * Date2);

/* It is used by the populationStatus function for the calculation of the number of non_vaccinated citizens per country for the specific virus.
 * 
 * Vlist: Virus list. (See Virus.h).
 * Country: Name of country.
 * Virus: Name of virus.
 * 
*/
int CalculatePopulation(Virus * Vlist, char * Country, char * Virus);

/* It is used by the CalculatedVacicinated function for the correct grouping by age for the vaccinated citizens.
 * 
 * Array: int array that is going to keep the number of vaccinated citizens per age group.
 * recordAge: Age of the citizen.
 * Vaccinated: boolean if the citizen is vaccinated.
 * 
*/
void InsertByAge(int * Array, int recordAge, bool Vaccinated);

/* It is used by the populationStatus function for printing of the statistics.
 * 
 * Country: name of the country.
 * Array: int array that is going to keep the number of vaccinated citizens per age group.
 * Age: boolean for the declaration between the populationStatus and populationStatusByAge.
 * Population: population of the country that it's statistics are going to be printed.
 * 
*/
void PrintCat(char * Country, int * Array, bool Age, int Population);