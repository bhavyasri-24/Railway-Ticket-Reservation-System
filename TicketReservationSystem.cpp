#include <bits/stdc++.h>

using namespace std;
class User;
class Admin;
class Passenger;
class Ticket;
class Train;

class User
{
private:
  string name;
  int age;
  string email;
  string mobile_no;
  string password;

protected:
  void getNameFromUser()
  {
    cout << "\nEnter name: " << endl;
    string tname;
    cin.ignore();
    getline(cin, tname);
    setName(tname);
  }

  void getAgeFromUser()
  {
    cout << "\nEnter Age: " << endl;
    int tage;
    cin >> tage;
     while (true) {
        try {
            cout << "\nEnter age of passenger: ";
            cin >> age;

            if (cin.fail() || age <= 18) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Invalid input! Please enter a valid age of passengers.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            break;
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
        }
    }
    setAge(tage);
  }

  void getPasswordFromUser()
  {
    string pas;

    while (true)
    {
      cout << "\nEnter password :\n(8-16 characters, including uppercase, lowercase, digit, and special character): " << endl;
      cin >> pas;
      try
      {
        if (validatePassword(pas))
        {
          setPassword(pas);
          break;
        }
      }
      catch (const exception &e)
      {
        cerr << "\ninvalid Password." << endl
             << e.what() << endl;
      }
    }
  }

  bool validatePassword(const string &pas) const
  {
    int len = pas.length();

    if (len < 8)
    {
      throw length_error("Password too Short");
    }
    if (len > 16)
    {
      throw length_error("Password too Long");
    }

    bool upper = false, lower = false, digit = false, special = false;

    for (char ch : pas)
    {
      if (isupper(ch))
        upper = true;
      else if (islower(ch))
        lower = true;
      else if (isdigit(ch))
        digit = true;
      else if (ispunct(ch))
        special = true;
    }
    if (!upper)
      throw invalid_argument("Password must contain at least one uppercase letter.");
    if (!lower)
      throw invalid_argument("Password must contain at least one lowercase letter.");
    if (!digit)
      throw invalid_argument("Password must contain at least one digit.");
    if (!special)
      throw invalid_argument("Password must contain at least one special character.");

    return true;
  }

public:
  User() : name(""), age(0), email(""), mobile_no(""), password("") {}
  // User(const string &email, const string &password) : email(email), password(password) {}
  User(const string &na, int ag) : name(na), age(ag) {}
  User(const string &na, const int ag, const string &em, const string &m_no, const string &pword) : name(na), age(ag), email(em), mobile_no(m_no), password(pword) {}

  string getName() const { return name; }
  string getEmail() const { return email; }
  string getPassword() const { return password; }
  string getMobileNo() const { return mobile_no; }
  int getAge() const { return age; }

  void setName(const string &sname) { name = sname; }
  void setEmail(const string &em) { email = em; }
  void setPassword(const string &pass) { password = pass; }
  void setAge(int sage) { age = sage; }
  void setMobileNo(const string &mn) { mobile_no = mn; }

  void virtual getEmailFromUser(){};
  void virtual getMobileNoFromUser(){};
  void virtual displayUserInfo() const {};
  void virtual saveUserDetails() const {};

  bool loginCheck(string email, string password)
  {
    if (this->email == email && this->password == password)
    {
      return true;
    }

    else
    {
      return false;
    }
  }

  friend class ticketReservationSystem;
};

class Passenger : public User
{
private:
  string berthPreference;
  int seatNumber;
  bool isLoggedIn;

public:
  Passenger() {};
  Passenger(string n, int a, string berthPref) : User(n, a), berthPreference(berthPref), seatNumber(0), isLoggedIn(false) {};
  Passenger(string n, int a, string berthPref, int seatNum, bool loggedIn)
      : User(n, a), berthPreference(berthPref), seatNumber(seatNum), isLoggedIn(loggedIn) {}

  string getBerthPreference() const { return berthPreference; }
  int getSeatNumber() const { return seatNumber; }
  void setBerthPreference(const string &pref) { berthPreference = pref; }
  void setSeatNumber(int snum) { seatNumber = snum; }

  void login()
  {
    isLoggedIn = true;
  }

  void logout()
  {
    isLoggedIn = false;
  }

  bool getLoginStatus()
  {
    return isLoggedIn;
  }

  void registerPassenger()
  {
    this->getNameFromUser();
    this->getAgeFromUser();
    User *bsPtr1 = this;
    (*bsPtr1).getEmailFromUser();
    (*bsPtr1).getMobileNoFromUser();
    this->getPasswordFromUser();

    User *bsPtr = this;
    (*bsPtr).saveUserDetails();

    cout << "\n\n\033[1mPassenger registered successfully!!\033[0m\n\n"
         << endl;
  }

  void saveUserDetails() const override
  {
    ofstream file("userdata.txt", ios::app); // Open file in append mode
    if (file.is_open())
    {
      file << "Name: " << this->getName() << endl;
      file << "Age: " << this->getAge() << endl;
      file << "Email: " << this->getEmail() << endl;
      file << "Password: " << this->getPassword() << endl;
      file << "Mobile_number: " << this->getMobileNo() << endl;
      file << "----------------------------" << endl;
      file.close();
      cout << "\nUser data saved to file (userdata.txt) successfully!" << endl;
    }
    else
    {
      cout << "Error opening file!" << endl;
    }
  }

  static vector<Passenger> loadUserDetails()
  {
    ifstream file("userdata.txt");
    vector<Passenger> users;

    if (!file.is_open())
    {
      cerr << "Error opening file!" << endl;
      return users;
    }

    string line;
    Passenger user;
    while (getline(file, line))
    {
      if (line.find("Name: ") != string::npos)
      {
        user.setName(line.substr(line.find(":") + 2));
      }
      else if (line.find("Age: ") != string::npos)
      {
        user.setAge(stoi(line.substr(line.find(":") + 2)));
      }
      else if (line.find("Email: ") != string::npos)
      {
        user.setEmail(line.substr(line.find(":") + 2));
      }
      else if (line.find("Password: ") != string::npos)
      {
        user.setPassword(line.substr(line.find(":") + 2));
      }
      else if (line.find("Mobile_number: ") != string::npos)
      {

        user.setMobileNo(line.substr(line.find(":") + 2));
      }
      else if (line == "----------------------------")
      {
        users.push_back(user);
        user = Passenger();
      }
    }

    file.close();
    return users;
  }

  void getEmailFromUser() override
  {
    cout << "\nEnter email: " << endl;
    string temail;
    cin >> temail;
    const regex emailPattern("^[a-z0-9._%+-]+@[a-z0-9-.]{2,}\\.[A-Za-z]{2,}$");

    while (!regex_match(temail, emailPattern))
    {
      cout << "\nInvalid email, try again: " << endl;
      cin >> temail;
    }

    fstream passengerFile;
    passengerFile.open("userdata.txt", ios::in);

    bool isEmailRegistered = false;

    if (passengerFile.is_open())
    {
      string line;

      while (getline(passengerFile, line))
      {
        size_t pos = line.find("Email:");
        if (pos != string::npos)
        {
          string registeredEmail = line.substr(pos + 6);

          registeredEmail.erase(registeredEmail.find_last_not_of(" \n\r\t") + 1);
          registeredEmail.erase(0, registeredEmail.find_first_not_of(" \n\r\t"));

          if (registeredEmail == temail)
          {
            cout << "\nThis email is already registered.\n\n";
            isEmailRegistered = true;
            break;
          }
        }
      }

      passengerFile.close();
    }
    if (isEmailRegistered)
    {
      getEmailFromUser();
    }
    else
    {
      setEmail(temail);
    };
  }

  void getMobileNoFromUser() override
  {
    string mn;
    cout << "\nEnter your mobile number (10 digits): \n";
    cin >> mn;

    while (mn.length() != 10)
    {
      cout << "\nEnter valid mobile number (10 digits): \n"
           << endl;
      cin >> mn;
    }

    fstream passengerFile;
    passengerFile.open("userdata.txt", ios::in);

    bool isMobNumRegistered = false;

    if (passengerFile.is_open())
    {
      string line;

      while (getline(passengerFile, line))
      {
        size_t pos = line.find("Mobile_number:");
        if (pos != string::npos)
        {
          string registeredMobNum = line.substr(pos + 14);

          registeredMobNum.erase(registeredMobNum.find_last_not_of(" \n\r\t") + 1);
          registeredMobNum.erase(0, registeredMobNum.find_first_not_of(" \n\r\t"));

          if (registeredMobNum == mn)
          {
            cout << "\nThis number is already registered.\n\n";
            isMobNumRegistered = true;
            break;
          }
        }
      }

      passengerFile.close();
    }
    if (isMobNumRegistered)
    {
      getMobileNoFromUser();
    }
    else
    {
      setMobileNo(mn);
    };
  }

  void displayUserInfo() const override
  {
    cout << "\n\nName: " << this->getName() << endl;
    cout << "Age: " << this->getAge() << endl;
    cout << "Email ID: " << this->getEmail() << endl;
    cout << "Mobile no.: " << this->getMobileNo() << endl;
  }
};

class Admin : public User
{
private:
  string position;
  int salary;
  bool isLoggedIn;

public:
  Admin() : User(), position(""), salary(0), isLoggedIn(false) {}

  string getPosition() const { return position; }
  int getSalary() const { return salary; }

  void setPosition(const string &pos) { position = pos; }
  void setSalary(int sal) { salary = sal; }

  void login()
  {
    isLoggedIn = true;
  }

  void logout()
  {
    isLoggedIn = false;
  }

  bool getLoginStatus()
  {
    return isLoggedIn;
  }

  void registerAdmin()
  {
    this->getNameFromUser();
    this->getAgeFromUser();
    User *bsPtr1 = this;
    (*bsPtr1).getEmailFromUser();
    (*bsPtr1).getMobileNoFromUser();
    this->getPasswordFromUser();

    string pos;
    int sal;
    cout << "\nEnter position of employee: ";
    cin.ignore();
    getline(cin, pos);
    this->position = pos;
    cout << "\nEnter salary of employee: ";
    cin >> sal;
    this->salary = sal;
    User *bsPtr = this;
    (*bsPtr).saveUserDetails();

    cout << "\n\n\033[1mAdmin registered successfully!!\033[0m\n\n"
         << endl;
  }

  void saveUserDetails() const override
  {
    ofstream adminFile("admin_data.txt", ios::app);
    if (adminFile)
    {
      adminFile << "Name:" << this->getName() << "\n"
                << "Age:" << this->getAge() << "\n"
                << "Email:" << this->getEmail() << "\n"
                << "Password:" << this->getPassword() << "\n"
                << "Mobile_number:" << this->getMobileNo() << "\n"
                << "Position:" << this->position << "\n"
                << "Salary:" << this->salary << "\n"
                << "----------------------------" << endl;
      ;
      adminFile.close();
      cout << "Admin details saved successfully.\n";
    }
    else
    {
      cerr << "Unable to open file to save admin details.\n";
    }
  }

  static vector<Admin> loadAdminDetails()
  {
    ifstream file("admin_data.txt");
    vector<Admin> admins;

    if (!file.is_open())
    {
      cerr << "Error opening admin data file!" << endl;
      return admins;
    }

    string line;
    Admin admin;
    while (getline(file, line))
    {
      if (line.find("Name:") != string::npos)
      {
        admin.setName(line.substr(line.find(":") + 1));
      }
      else if (line.find("Age:") != string::npos)
      {
        admin.setAge(stoi(line.substr(line.find(":") + 1)));
      }
      else if (line.find("Email:") != string::npos)
      {
        admin.setEmail(line.substr(line.find(":") + 1));
      }
      else if (line.find("Password:") != string::npos)
      {
        admin.setPassword(line.substr(line.find(":") + 1));
      }
      else if (line.find("Mobile_number:") != string::npos)
      {
        admin.setMobileNo(line.substr(line.find(":") + 1));
      }
      else if (line.find("Position:") != string::npos)
      {
        admin.setPosition(line.substr(line.find(":") + 1));
      }
      else if (line.find("Salary:") != string::npos)
      {
        admin.setSalary(stoi(line.substr(line.find(":") + 1)));
      }
      else if (line == "----------------------------")
      {
        admins.push_back(admin);
        admin = Admin();
      }
    }

    file.close();
    return admins;
  }

  void getEmailFromUser() override
  {
    cout << "\nEnter email: " << endl;
    string temail;
    cin >> temail;
    const regex emailPattern("^[a-z0-9._%+-]+@[a-z0-9-.]{2,}\\.[A-Za-z]{2,}$");

    while (!regex_match(temail, emailPattern))
    {
      cout << "\nInvalid email, try again: " << endl;
      cin >> temail;
    }

    fstream AdminFile;
    AdminFile.open("admin_data.txt", ios::in);

    bool isEmailRegistered = false;

    if (AdminFile.is_open())
    {
      string line;

      while (getline(AdminFile, line))
      {
        size_t pos = line.find("Email:");
        if (pos != string::npos)
        {
          string registeredEmail = line.substr(pos + 6);

          registeredEmail.erase(registeredEmail.find_last_not_of(" \n\r\t") + 1);
          registeredEmail.erase(0, registeredEmail.find_first_not_of(" \n\r\t"));

          if (registeredEmail == temail)
          {
            cout << "\nThis email is already registered.\n\n";
            isEmailRegistered = true;
            break;
          }
        }
      }

      AdminFile.close();
    }
    if (isEmailRegistered)
    {
      getEmailFromUser();
    }
    else
    {
      setEmail(temail);
    };
  }

  void getMobileNoFromUser() override
  {
    string mn;
    cout << "\nEnter your mobile number (10 digits): \n";
    cin >> mn;

    while (mn.length() != 10)
    {
      cout << "\nEnter valid mobile number (10 digits): \n"
           << endl;
      cin >> mn;
    }

    fstream AdminFile;
    AdminFile.open("admin_data.txt", ios::in);

    bool isMobNumRegistered = false;

    if (AdminFile.is_open())
    {
      string line;

      while (getline(AdminFile, line))
      {
        size_t pos = line.find("Mobile_number:");
        if (pos != string::npos)
        {
          string registeredMobNum = line.substr(pos + 14);

          registeredMobNum.erase(registeredMobNum.find_last_not_of(" \n\r\t") + 1);
          registeredMobNum.erase(0, registeredMobNum.find_first_not_of(" \n\r\t"));

          if (registeredMobNum == mn)
          {
            cout << "\nThis number is already registered.\n\n";
            isMobNumRegistered = true;
            break;
          }
        }
      }

      AdminFile.close();
    }
    if (isMobNumRegistered)
    {
      getMobileNoFromUser();
    }
    else
    {
      setMobileNo(mn);
    };
  }

  void displayUserInfo() const override
  {
    cout << "\n\nName: " << this->getName() << endl;
    cout << "Age: " << this->getAge() << endl;
    cout << "Email ID: " << this->getEmail() << endl;
    cout << "Mobile no.: " << this->getMobileNo() << endl;
    cout << "Position: " << position << endl;
    cout << "Salary: " << salary << " per month" << endl;
  }
};

class Train
{
private:
  static int trainNumber;
  string trainName;
  vector<string> stops;
  int totalSeats;
  vector<int> seatStatus;
  int currentTrainNumber;

public:
  Train() : trainName(""), totalSeats(0), currentTrainNumber(0)
  {
    seatStatus.clear();
  }
  Train(string Tname, vector<string> stations, int seats) : trainName(Tname), stops(stations), totalSeats(seats), seatStatus(seats, 0)
  {
    initializeTraincounter();
    currentTrainNumber = ++trainNumber;
  }

  static void initializeTraincounter()
  {
    trainNumber = fetchLastTrainNum();
  }

  int getTrainNumber() const
  {
    return currentTrainNumber;
  }
  string getTrainName()
  {
    return trainName;
  }
  int getAvailableSeats() const
  {
    int count = 0;
    for (int status : seatStatus)
    {
      if (status == 0)
      {
        count++;
      }
    }
    return count;
  }
  void setTrainNumber(int trainNum)
  {
    currentTrainNumber=trainNum;
  }

  static vector<Train> loadTrainDetails()
  {
    vector<Train> trains;
    ifstream trainFile("train_data.txt"); // Make sure this matches the save function's file

    if (trainFile)
    {
      string line;
      while (getline(trainFile, line))
      {
        istringstream iss(line);
        string tNumber, tName, tSeatsStr, stop;
        int tSeats;
        vector<string> stopList;
        vector<int> seatStatusList;

        
        getline(iss, tNumber, ',');
        
        getline(iss, tName, ',');
       
        getline(iss, tSeatsStr, ',');
        tSeats = stoi(tSeatsStr); 

        // Read stops
        while (getline(iss, stop, ','))
        {
          if (isdigit(stop[0]))
          {        
            break; 
          }
          stopList.push_back(stop);
        }

       
        string seatStatusStr;
        do
        {
          if (!stop.empty())
          {                                       
            seatStatusList.push_back(stoi(stop)); 
          }
          while (getline(iss, seatStatusStr, ','))
          {
            if (!seatStatusStr.empty())
            {
              seatStatusList.push_back(stoi(seatStatusStr)); 
            }
          }
        } while (false); 

        
        Train train;
        train.currentTrainNumber = stoi(tNumber); 
        train.trainName = tName;                 
        train.totalSeats = tSeats;                
        train.stops = stopList;                   
        train.seatStatus = seatStatusList;       

        
        trains.push_back(train);
      }

      trainFile.close();
    }
    else
    {
      cerr << "Unable to open file to load train details.\n";
    }

    return trains; 
  }

  static bool searchTrain(string src, string dest)
  {
    vector<Train> availableTrains = loadTrainDetails();
    bool trainFound = false;
    for (auto train : availableTrains)
    {
      if (train.existsBetStations(src, dest) != 0)
      {
        cout << "\nTrain found between " << src << " and " << dest << endl
             << endl;
        train.displayTrainDetails();
        cout << endl;
        trainFound = true;
      }
    }

    if (!trainFound)
    {
      cout << "\n\nNo train found between " << src << " and " << dest << endl
           << endl;
    }

    return trainFound;
  }

  int existsBetStations(string src, string dest)
  {
    int srcindex = -1, destindex = -1;
    // bool srcfound=false;
    for (int i = 0; i < stops.size(); i++)
    {
      if (src == stops[i])
      {
        srcindex = i;
      }
      if (dest == stops[i])
      {
        destindex = i;
      }

      if (srcindex != -1 && destindex != -1 && srcindex < destindex)
      {
        cout << "no of stations in between" << destindex - srcindex << endl;
        return destindex - srcindex;
      }
    }

    return 0;
  }

  void newTrain()
  {

    int noOfStations;

    cout << "\nEnter train name: ";
    cin.ignore();
    getline(cin, trainName);
    if (trainName.length() > 100)
    {
      throw std::length_error("\nTrain name is too long!");
    }

    while (true) {
        try {
            cout << "\nEnter total number of stations: ";
            cin >> noOfStations;

            if (cin.fail() || noOfStations < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Invalid input! Please enter a valid Number of Seats.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            break;
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
        }
    }

    cout << "\nEnter all stops: ";
    while(noOfStations <=  0)
    {
        cout<<"Invalid input!  Please enter valid number of stations."<<endl;
        cin>>noOfStations;
    }
    for (int i = 0; i < noOfStations; i++)
    {
      string stop;
      
      while(true)
      {
        try
        {
            cin>>stop;
            bool hasletter = false;
            bool hasonlydigits = true;

            for(char z1: stop)
            {   
                if(isalpha(z1)){hasletter = true;}
                if(!isdigit(z1)){hasonlydigits = false;}
            }   
            if(!hasletter || hasonlydigits)
            {
                throw invalid_argument("invalid input! Enter  a valid station/stop name");
            }
            stops.push_back(stop);
            break;
        }
        catch (const invalid_argument &e)
        {
            cout << e.what() << endl;
        }
      }
    }

   while (true) {
        try {
            cout << "\nEnter total seats: ";
            cin >> totalSeats;

            if (cin.fail() || totalSeats < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Invalid input! Please enter a valid Number of Seats.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            break;
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
        }
    }
    seatStatus.resize(totalSeats); // Resize the vector to hold total seats
    for (int i = 0; i < totalSeats; i++)
    {
      seatStatus[i] = 0; // 0 for available
    }
    cout << "done\n";

    currentTrainNumber = fetchLastTrainNum() + 1;
    trainNumber = currentTrainNumber;
    saveTrainDetails();
  }

  static int fetchLastTrainNum()
  {
    ifstream file("train_data.txt", ios::in);
    if (!file)
    {
      cerr << "Error opening file!" << endl;
      return 10000; 
    }

    string line;
    int lastTrainNumber = 10000; 

   
    while (getline(file, line))
    {
      if (!line.empty())
      {
        istringstream iss(line);
        string trainNumberStr;
        getline(iss, trainNumberStr, ',');      
        lastTrainNumber = stoi(trainNumberStr); 
      }
    }

    file.close();
    return lastTrainNumber; 
  }

  int assignSeat(string berthPreference)
  {
    for (int i = 0; i < seatStatus.size(); i++)
    {
      if ((berthPreference == "lower" && i % 2 == 1 && seatStatus[i] == 0) || (berthPreference == "upper" && i % 2 == 0 && seatStatus[i] == 0))
      {
        seatStatus[i] = 1;
        return i + 1;
      }
    }
    for (int i = 0; i < seatStatus.size(); i++)
    {
      if (seatStatus[i] == 0)
      {
        seatStatus[i] = 1;
        return i + 1;
      }
    }

    return -1;
  }

  void cancelSeat(int seatNumber)
  {
    if (seatNumber < 1 || seatNumber > seatStatus.size())
    {
      cout << "\nInvalid seat number: " << seatNumber << ". Please enter a valid seat number.\n";
      return;
    }
    if (seatStatus[seatNumber - 1] == 0)
    {
      cout << "\nSeat " << seatNumber << " is already available.\n";
      return;
    }
    seatStatus[seatNumber - 1] = 0;
    cout << "\nSeat number " << seatNumber << " is cancelled.\n";
  }

  void displayTrainDetails()
  {
    cout << "\nTrain details\n"
         << endl;
    cout << "Train Number: " << currentTrainNumber << endl;
    cout << "Train Name: " << trainName << endl;
    cout << "Available seats: " << getAvailableSeats() << endl;
  }

  void saveTrainDetails() const
  {
    std::ofstream trainFile("train_data.txt", ios::app);
    if (trainFile)
    {
      trainFile << currentTrainNumber << "," << trainName << "," << totalSeats << ",";

      for (size_t i = 0; i < stops.size(); ++i)
      {
        trainFile << stops[i];
        if (i != stops.size() - 1)
          trainFile << ",";
      }

      trainFile << ","; 

      
      for (int seat : seatStatus)
      {
        trainFile << seat << ","; 
      }
      trainFile << "\n"; 
      std::cout << "Train details saved successfully.\n";
    }
    else
    {
      std::cerr << "Unable to open file to save train details.\n";
    }
  }

void updateTrainDetails() const {
   
    vector<Train> trains = loadTrainDetails();
    
    
    ofstream trainFile("train_data.txt", ios::out);
    if (!trainFile) {
        cerr << "Unable to open file to update train details.\n";
        return;
    }
    

    for (auto &train : trains) {
        if (train.currentTrainNumber == this->currentTrainNumber) {
      
            train = *this;
        }
        

        trainFile << train.currentTrainNumber << "," << train.trainName << "," << train.totalSeats << ",";
        

        for (size_t i = 0; i < train.stops.size(); ++i) {
            trainFile << train.stops[i];
            if (i != train.stops.size() - 1)
                trainFile << ",";
        }
        
        trainFile << ","; 
      
        for (int seat : train.seatStatus) {
            trainFile << seat << ",";
        }
        trainFile << "\n";
    }
    
    trainFile.close();
    cout << "Train details updated successfully.\n";
}

};

class Ticket
{
private:
  static long long PNRcounter;
  long long PNRnumber;
  Train *train;
  vector<Passenger> passengers;
  vector<int> seatNumbers;
  int totalPrice;
  string source, destination;
  bool isCancelled;

public:
  Ticket() : train(nullptr), source(""), destination(""), totalPrice(0), isCancelled(false)
  {
    initializePNRcounter();
    PNRnumber = ++PNRcounter;
  }
  Ticket(Train *t, string src, string dest) : train(t), source(src), destination(dest), totalPrice(0), isCancelled(false)
  {
    initializePNRcounter();
    PNRnumber = ++PNRcounter;
  };

  void setValues(long long pnr, const string &trainNum, const string &src, const string &dest, const vector<Passenger> &pass)
  {
    PNRnumber = pnr;
    if(train!=nullptr)
    train->setTrainNumber(stoi(trainNum));
    source = src;
    destination = dest;
    passengers = pass; 
  }

  static void initializePNRcounter()
  {
    PNRcounter = fetchLastPNR();
  }

  void displayTicketDetails()
  {
    cout << "\nPNR number: " << PNRnumber << endl;
    cout << "Train number: " << train->getTrainNumber() << endl;
    cout << "Train name: " << train->getTrainName() << endl;
    cout << "Source: " << source << endl;
    cout << "Destination: " << destination << endl;
    for (int i = 0; i < passengers.size(); i++)
    {
      cout << "\nPassenger " << i + 1 << ":\n";
      cout << "-----------------------" << endl;
      cout << "Name: " << passengers[i].getName() << endl;
      cout << "Age: " << passengers[i].getAge() << endl;
      cout << "seat number: " << seatNumbers[i] << endl;
      if (seatNumbers[i] % 2 == 0)
      {
        cout << "Berth: Upper" << endl;
      }
      else
      {
        cout << "Berth: Lower" << endl;
      }
    }
    cout << "\nTotal Price: " << totalPrice << endl;
  }

  void addPassenger(Passenger p)
  {
    passengers.push_back(p);
    return;
  }

  void calculatePrice(Train *train)
  {
    int baseCost = 80;
    int additionalCost = 20;

    int stationCount = (train)->existsBetStations(this->source, this->destination);
    cout << "calculated stations in between" << stationCount << endl;
    if (stationCount <= 0)
    {
      cout << "\nInvalid source and destination\n\n";
      return;
    }
    else
    {
      totalPrice = baseCost;
      if (stationCount > 3)
      {
        totalPrice += (stationCount - 3) * additionalCost;
      }
    }

    totalPrice *= passengers.size();
  }

  void bookTicket(string src, string dest)
  {
    this->source = src;
    this->destination = dest;
    if (Train::searchTrain(src, dest))
    {
      vector<Train> availableTrains = Train::loadTrainDetails();
      int selectedTrainNumber;
      cout << "\nEnter train number: ";
      cin >> selectedTrainNumber;

      Train *selectedTrain = nullptr;
      for (auto &train1 : availableTrains)
      {
        if (selectedTrainNumber == train1.getTrainNumber())
        {
          selectedTrain = &train1;
          break; 
        }
      }
      this->train = selectedTrain;

      if (selectedTrain == nullptr)
      {
        cout << "\nInvalid train number selected." << endl;
        return;
      }

      int availableSeats = selectedTrain->getAvailableSeats();
      int numOfPassengers;
     while (true) {
        try {
            cout << "\nEnter number of passengers: ";
            cin >> numOfPassengers;

            if (cin.fail() || numOfPassengers <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Invalid input! Please enter a valid number of passengers.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            break;
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
        }
    }
    

      if (numOfPassengers > availableSeats)
      {
        cout << "\nNot enough seats available. Only " << availableSeats << " seats are available." << endl;
        return;
      }

      cout << "\nPNR number: " << this->getPNR() << endl;

      for (int i = 1; i <= numOfPassengers; i++)
      {
        string name, berthPreference;
        int age, berthChoice;

        cout << "\nEnter name of passenger " << i << " :";
        cin >> name;
 
        while (true) {
        try {
            cout << "\nEnter age of passenger: ";
            cin >> age;

            if (cin.fail() || age <= 18) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Invalid input! Please enter a valid age of passengers.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            break;
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
        }
    }

        cout << "\nChoose berth preference for " << name << " :\n1)Lower\n2)Upper\n";
        cin >> berthChoice;

        berthPreference = (berthChoice == 1) ? "lower" : "upper";
        Passenger p(name, age, berthPreference);
        this->addPassenger(p);
      }

    
      if (selectedTrain->getAvailableSeats() >= passengers.size())
      {
        for (auto &passenger : passengers)
        {
          int seatNumber = selectedTrain->assignSeat(passenger.getBerthPreference());
          if (seatNumber != -1) 
          {
            cout << seatNumber << endl;
            seatNumbers.push_back(seatNumber);
            passenger.setSeatNumber(seatNumber);
          }
          else
          {
            cout << "\nCould not assign seat to passenger " << passenger.getName() << endl;
          }
        }

        calculatePrice(selectedTrain);
        if (totalPrice > 0)
        {
          cout << "\n\n\033[1mTicket booked successfully for " << passengers.size() << " passengers!!\033[0m\n\n";
          this->displayTicketDetails();

          // Save booking details to the file
          saveBookingDetails();

          selectedTrain->updateTrainDetails();
        }
      }
      else
      {
        cout << "\nNot enough seats available\n\n";
      }
    }
  }

  void cancelTicket()
  {
    if (isCancelled)
    {
      cout << "\nTicket is already cancelled.\n";
      return;
    }

    int choice;
    cout << "\n\nChoose cancel option:\n\n1)Cancel all seats\n2)Cancel specific seats\n";
    cin >> choice;

    if (choice == 1)
    {
      for (auto seat : seatNumbers)
      {
        train->cancelSeat(seat);
      }
      seatNumbers.clear();
      passengers.clear();
      isCancelled = true;
      cout << "\n\nAll seats associated with this ticket have been cancelled\n";
    }
    else if (choice == 2)
    {
      cout << "\nSeats that are available for cancellation: \n";
      for (int i = 0; i < seatNumbers.size(); i++)
      {
        cout << passengers[i].getName() << ": " << seatNumbers[i] << endl;
      }

      cout << "\nEnter seat number you want to cancel (or 0 to finish): ";
      int seatChoice;
      while (1)
      {
        cin >> seatChoice;

        if (seatChoice == 0)
        {
          break;
        }

        if (seatChoice >= 1 && seatChoice <= seatNumbers.size())
        {
          int seatToCancel = seatNumbers[seatChoice - 1];
          train->cancelSeat(seatToCancel);
          seatNumbers.erase(seatNumbers.begin() + seatChoice - 1);
          passengers.erase(passengers.begin() + seatChoice - 1);
          cout << "Seat " << seatToCancel << " is cancelled\n";
        }
        else
        {
          cout << "\nInvalid choice";
        }

        cout << "\nEnter seat number you want to cancel (or 0 to finish): ";
      }
    }
  }
    static void pnrEnquiry() {
    string pnr;
    cout << "Enter PNR number to inquire: ";
    cin >> pnr; 

    ifstream ticketFile("ticket_data.txt");
    if (!ticketFile) {
        cerr << "Unable to open file to read booking details.\n";
        return;
    }

    string line;
    bool found = false;

    while (getline(ticketFile, line)) {
        istringstream ss(line);
        string token;

        getline(ss, token, '|');
        if (token == pnr) {
            found = true;
            string trainNumber, source, destination;

            getline(ss, trainNumber, '|');
            getline(ss, source, '|');
            getline(ss, destination, '|');

            cout << "\nBooking details for PNR " << pnr << ":\n";
            cout << "Train Number: " << trainNumber << "\n";
            cout << "Source: " << source << "\n";
            cout << "Destination: " << destination << "\n";
            cout << "Passengers:\n";
        
            while (getline(ss, token, '|')) {
                istringstream passengerStream(token);
                string name, age, seatNumber;

                getline(passengerStream, name, '|');
                getline(passengerStream, age, '|');
                getline(passengerStream, seatNumber, '|');
                
                if (!name.empty() || !age.empty() || !seatNumber.empty()) {
                    cout << "  Name: " << name << "\n";
                    cout << "  Age: " << age << "\n";
                    cout << "  Seat Number: " << seatNumber << "\n";
                }
            }
            break; 
        }
    }

    if (!found) {
        cout << "Invalid PNR number\n";
    }
}

  static Ticket *getTicketByPNR()
  {
    long long PNR;
    cin >> PNR;

    vector<Ticket> tickets = loadBookingDetails();

    for (auto &ticket : tickets)
    {
      if (PNR == ticket.getPNR())
      {
        return &ticket;
      }
    }

    return nullptr;
  }

  long long getPNR()
  {
    return PNRnumber;
  }

  static long long fetchLastPNR()
  {
    ifstream file("ticket_data.txt", ios::in);
    if (!file)
    {
      cerr << "Error opening file!" << endl;
      return 4000000000;
    }

    string line;
    string lastLine;

    while (getline(file, line))
    {
      if (!line.empty())
      {
        lastLine = line;
      }
    }

    file.close();

    if (lastLine.empty())
    {
      return 4000000000;
    }
    string pnr;
    istringstream iss (lastLine);
    getline(iss, pnr , '|');
    if (!pnr.empty())
    {
      return stoll(pnr);
    }
    else
    {
      return 4000000000;
    }
  }

  void saveBookingDetails() const
  {
    ofstream ticketFile("ticket_data.txt", ios::app);
    if (ticketFile)
    {
      ticketFile <<"\n"<< PNRnumber << "|"
                 << this->train->getTrainNumber() << "|"
                 << source << "|"
                 << destination << "|";
      for (auto &passenger : passengers)
      {
        ticketFile << passenger.getName() << "|"
                   << passenger.getAge() << "|"
                   << passenger.getSeatNumber() << "|";
      }
      cout<<endl;
      ticketFile.close();
      cout << "Booking details saved successfully.\n";
    }
    else
    {
      cerr << "Unable to open file to save booking details.\n";
    }
  }

  static vector<Ticket> loadBookingDetails()
  {
    vector<Ticket> tickets;
    ifstream ticketFile("ticket_data.txt");
    string line;

    if (ticketFile.is_open())
    {
      while (getline(ticketFile, line))
      {
        istringstream iss(line);
        Ticket ticket;
        string pnr, trainNum, source, destination;

        
        getline(iss, pnr, '|');
        getline(iss, trainNum, '|');
        getline(iss, source, '|');
        getline(iss, destination, '|');

        vector<Passenger> passengers;

     
        while (!iss.eof())
        {
          string name;
          int age, seat;

       
          getline(iss, name, '|');
          if (name.empty())
            break; 

   
          string ageStr;
          getline(iss, ageStr, '|');
          age = stoi(ageStr); 

          
          string seatStr;
          getline(iss, seatStr, '|');
          seat = stoi(seatStr);

          
          Passenger passenger(name, age, "", seat, false);
          passengers.push_back(passenger);
        }

        
        ticket.setValues(stoll(pnr), trainNum, source, destination, passengers);

        tickets.push_back(ticket);
      }
      ticketFile.close();
    }
    else
    {
      cerr << "Unable to open file to load booking details.\n";
    }

    return tickets;
  }

friend class ticketReservationSystem;
};

class ticketReservationSystem
{
private:
  Passenger *loggedInPassengerUser = nullptr;
  Admin *loggedInAdmin = nullptr;

public:
  Passenger *loginPassengerUser()
  {
    string loginEmail, loginPassword;
    cout << "\nEnter the login email: " << endl;
    cin >> loginEmail;
    cout << "\nEnter the Password: " << endl;
    cin >> loginPassword;

    vector<Passenger> registeredPassengers = Passenger::loadUserDetails();

    for (auto &passenger : registeredPassengers)
    {
      if (passenger.loginCheck(loginEmail, loginPassword))
      {
        loggedInPassengerUser = &passenger;
        passenger.login();
        cout << "\n\n\033[1mLogin successfull!!\033[0m\n\n"
             << endl;
        return loggedInPassengerUser;
      }
    }

    cout << "\n\nInvalid credentials./ Register if not registered.\n\n"
         << endl;
    return nullptr;
  }

  Admin *loginAdmin()
  {
    string loginEmail, loginPassword;
    cout << "\nEnter the login email: " << endl;
    cin >> loginEmail;
    cout << "\nEnter the Password: " << endl;
    cin >> loginPassword;

    vector<Admin> registeredAdmins = Admin::loadAdminDetails();

    for (auto &admin : registeredAdmins)
    {
      if (admin.loginCheck(loginEmail, loginPassword))
      {
        loggedInAdmin = &admin;
        admin.login();
        cout << "\n\n\033[1mLogin successfull!!\033[0m\n\n"
             << endl;
        return loggedInAdmin;
      }
    }
    cout << "\n\nInvalid credentials./ Register if not registered.\n\n"
         << endl;
    return loggedInAdmin;
  }

  void logout(bool isAdmin)
  {
    if (isAdmin)
    {
      loggedInAdmin = nullptr;
      cout << "\n\nAdmin logged out successfully!\n\n"
           << endl;
    }
    else
    {
      loggedInPassengerUser = nullptr;
      cout << "\n\nPassenger logged out successfully!\n\n"
           << endl;
    }
  }

  bool isPassengerLoggedIn()
  {
    return loggedInPassengerUser != nullptr;
  }

  bool isAdminLoggedIn()
  {
    return loggedInAdmin != nullptr;
  }

  void adminMenu(Admin *admin)
  {
    while (isAdminLoggedIn())
    {
      cout << "\n\033[1mAdmin Menu\033[0m\n\n";
      cout << "1) Add train\n";
      cout << "2) Search Train\n";
      cout << "3) PNR enquiry\n";
      cout << "4) Display all trains\n";
      cout << "5) Display all registered admins\n";
      cout << "6) Display all registered users\n";
      cout << "7) Register Admin\n";
      cout << "8) Logout\n";

      int adminChoice;
      cin >> adminChoice;

      switch (adminChoice)
      {
      case 1:
      {
        Train t;
        t.newTrain();
        break;
      }

      case 2:
      {
        string src, dest;
        cout << "Enter source: ";
        cin.ignore();
        getline(cin, src);
        cout << "Enter destination: ";
        getline(cin, dest);

        Train::searchTrain(src, dest);
        break;
      }
      case 3:
        Ticket::pnrEnquiry();
        break;

      case 4:
      {
        vector<Train> trains = Train::loadTrainDetails();
        for (auto train : trains)
        {
          train.displayTrainDetails();
        }
        break;
      }
      case 5:
      {
        vector<Admin> admins = Admin::loadAdminDetails();
        for (auto admin : admins)
        {
          User *bsPtr = &admin;
          (*bsPtr).displayUserInfo();
        }
        break;
      }

      case 6:
      {
        vector<Passenger> passengers = Passenger::loadUserDetails();
        for (auto passenger : passengers)
        {
          User *bsPtr = &passenger;
          (*bsPtr).displayUserInfo();
        }
        break;
      }

      case 7:
      {
        Admin a;
        a.registerAdmin();
        break;
      }
      case 8:
        logout(true);
        admin->logout();
        break;
      }
    }
  }

  void passengerMenu(Passenger *passengerUser)
  {
    while (isPassengerLoggedIn())
    {
      cout << "\n\033[1mUser Menu\033[0m\n";
      cout << "1) Search trains\n";
      cout << "2) Book Ticket\n";
      cout << "3) Cancel seat\n";
      cout << "4) PNR enquiry\n";
      cout << "5) Logout\n";

      int userChoice;
      cin >> userChoice;

      switch (userChoice)
      {
      case 1:
      {
        string src, dest;
        cout << "Enter source: ";
        cin.ignore();
        getline(cin, src);
        cout << "Enter destination: ";
        getline(cin, dest);
        Train::searchTrain(src, dest);
        break;
      }
      case 2:
      {
        string src, dest;
        cout << "Enter source: ";
        cin.ignore();
        getline(cin, src);
        cout << "Enter destination: ";
        getline(cin, dest);

        Ticket t;
        t.bookTicket(src, dest);
      }
      break;
      case 3:
      {
        cout << "Enter PNR number: ";
        Ticket *ticket = Ticket::getTicketByPNR();
        if (ticket != nullptr)
        {
          ticket->cancelTicket();
        }
        else
        {
          cout << "Invalid PNR number\n";
        }
        break;
      }
      case 4:
        Ticket::pnrEnquiry();
        break;

       case 5:
        logout(false);
        passengerUser->logout();
        break;
      }
    }
  }

  void showMainMenu()
  {
    while (true)
    {
      cout << "Main Menu:\n";
      cout << "1. Register User\n";
      cout << "2. Login as Passenger\n";
      cout << "3. Login as Admin\n";
      cout << "4. Exit\n";

      int choice;
      cout << "Enter your choice: ";
      cin >> choice;

      switch (choice)
      {
      case 1:
      {
        Passenger p;
        p.registerPassenger();
        break;
      }
      case 2:
      {
        loggedInPassengerUser = loginPassengerUser();
        if (loggedInPassengerUser)
        {
          passengerMenu(loggedInPassengerUser);
        }
        break;
      }
      case 3:
      {
        loggedInAdmin = loginAdmin();
        if (loggedInAdmin)
        {
          adminMenu(loggedInAdmin);
        }
        break;
      }
      case 4:
        cout << "Exiting...\n";
        return;
      default:
        cout << "Invalid choice! Please try again.\n";
      }
    }
  }
};

int Train::trainNumber = 10000;
long long Ticket::PNRcounter = 4000000000;

int main()
{
  ticketReservationSystem system;
  system.showMainMenu();
  return 0;
}