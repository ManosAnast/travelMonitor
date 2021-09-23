# Goal:
 travelMonitor is a program that keeps a record for every citizen and the vaccinations that they have done and based on their vaccination status, it allows them to travel to an other country.
 Linux terminal application

# Program call:
  
First compile the code with:
    
    make

  Then, the program is called with this format:
    
    ./travelMonitor –m numMonitors -b bufferSize -s sizeOfBloom -i input_dir

    – numMonitors is the number of Monitor processes that are going to be used.

    – bufferSize is the number of bytes that pipes are going to read.
    
    – sizeOfBloom is the number of bytes that the bloom filter will have.
    
    – input_dir directory that has the citizens.

File format:
  ID FirstName LastName Country Age Virus vaccinationStatus(YES/NO) Date(If vaccinationStatus is YES)

Script:

  Call format:
    
    ./create_infiles.sh inputFile input_dir numFilesPerDirectory
    
    - inputFile: file that contains the citizens.(Based on file format)
    
    - input_dir: the directory that will contain all the subdirectories and the input files.
    
    - numFilesPerDirectory: number of files per directory.
    
# Commands:

    ● /travelRequest citizenID date countryFrom countryTo virusName
    
  Check if the citizen with the given citizenID is vaccinated for the virusName. If she/he is vaccinated within 6 months then the travel request is accepted. Otherwise, the request is rejected.

  Output format:
   REQUEST REJECTED – YOU ARE NOT VACCINATED *OR*
   REQUEST REJECTED – YOU WILL NEED ANOTHER VACCINATION BEFORE TRAVEL DATE *OR*
   REQUEST ACCEPTED – HAPPY TRAVELS 
   
    ● /travelStats virusName date1 date2 [country]
    
  If no country argument is given, the application will print the number of citizens who have requested approval to
travel for the period [date1 ... date2] to countries that check the vaccination for a virusName, and the number of citizens approved and rejected. If the country argument is given, print the same information but only for the specific country.

  Output format:
   TOTAL REQUESTS 29150
   ACCEPTED 25663
   REJECTED 3487

    ● /addVaccinationRecords country
    
  With this command the user can insert a new country subdirectory to input_dir.

  Output format: 
  *Nothing*

    ● /searchVaccinationStatus citizenID
  
  Search for the vaccination status of the citizen with citizenID.

   Output format: 
  889 MANOS ANASTASAKIS GREECE
  AGE 52
  COVID-19 VACCINATED ON 27-12-2020
  H1N1 NOT YET VACCINATED

    ● /exit
    
  The application is terminated with and makes a Log file where you can see all the countries that the application was checking and the requests that was made.
  
   Log file format:
  Greece
  Italy
  France
  TOTAL TRAVEL REQUESTS 883832
  ACCEPTED 881818
  REJECTED 2014
