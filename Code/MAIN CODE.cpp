#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;
//manager database: name password
//room database: room_type, room per night charge, No of Rooms
//menu database: item number, item name, item price
//customer database: cust name, cust room, cust o/s payment
template <typename T>
T valid(T low, T high, T input){
    while(input<low || input>high){
        cout<<"\n INVALID INPUT! ";                         
              cout<<"\n Range: ("<<low<<" - " <<high<< ")\nEnter Valid Input: ";
        cin>>input;
    }
    return input;
}
class manager{
    string manager_name, manager_password;
    public:
        void login()
        {
            int i;
            string username, password;
            cout<<"\n ====== LOGIN ====== \n Username: ";
            cin>>manager_name;
            cout<<"Password: ";
            cin>>manager_password;
            ifstream ifile;
            ifile.open("Manager_Database.txt");
            if(ifile.fail()){
                cout<<"\n Login Failed due to file error. Please check the file and run the code again! ";
                exit(1);
            }
            else{
                ifile>>username>>password;
            }
            i = 5;
            while(i != 0){
                if(manager_name==username && manager_password== password){
                    cout<<"\n LOGIN SUCCESSFUL! \n Welcome Manager!";
                    break;
                }
                else{ 
                    i--;
                    if (i==0){
                        cout<<"\n UNABLE TO LOGIN. Session Ended.";
                        exit(1);
                    }
                    cout<<"\n Username or password incorrect. Please try again! \n "<<i<<" tries remaining \n Username: ";
                    cin>>manager_name;
                    cout<<"Password: ";
                    cin>>manager_password;
                }
        }
    }
};
const int types = 6; //types of rooms
class reception{
    string cust_name, mobile_no;
    int room_no, verification_status, stay_duration, room_choice, customerindex; //Status of Customer's record in the database
    int room_receipt;
    //room database variables
    string roomdata_roomtype[types];
    int roomdata_no_of_rooms[types] = {0};
    int roomdata_roomprice[types]= {0};
    //customer database variables
    string customerdata_name[51];
    int customerdata_roomno[51] = {0}, customerdata_roomtype[51];
    int customerdata_outstanding[51] = {0};
    public:
        void check_in(){    
            verification_status = 0;
            cout<<"\n ====== CHECK-IN PORTAL ====== \n";
            cout<<"\n Customer Details: ";
            cout<<"\nFull Name: ";
            cin.ignore(); //to clear any whitespace from the console.
            getline(cin, cust_name);
            verify_cust_data(1); //verifies if the customer has already checked-in or not.
            if (verification_status == 1){
                cout<<"\n Customer has already checked-in!! ";
                return;
            }
            cout<<"Mobile Number: ";
            getline(cin, mobile_no);
            RoomBrochure: //Destination for the Brochure
            cout<<" \n \n ======= Brochure ====== ";
            get_roomdata();
            cout<<"\n   Type of Room - Price of Room per Night (Rs.) "<<endl;
            for(int i = 0; i < types; i++){
                cout<<i+1<<". "<<roomdata_roomtype[i]<<" -\t"<<roomdata_roomprice[i]<<endl;
            }
            cout<<"\n Enter Room Preference (1-6): ";
            cin>>room_choice;
            room_choice = valid(1, 6, room_choice);
            room_choice = room_choice - 1;
            if (roomdata_no_of_rooms[room_choice] == 0){
                int choice;
                cout<<"\n Room unavailable at the moment! \n1. Choose another Type of Room \n2. Exit \n Choice: ";
                cin>>choice;
                choice = valid(1,2,choice);
                if(choice == 1){
                    goto RoomBrochure;
                }
                else{
                    return;
                }
            }
            cout<<"\n Duration of Stay (days): ";
            cin>>stay_duration;
            stay_duration = valid(1, 15, stay_duration);
            room_no = ((room_choice+1)*100)+roomdata_no_of_rooms[room_choice]; //generates room number
            cout<<"\n Room No.: "<<room_no<<" assigned.";
            roomdata_no_of_rooms[room_choice] = roomdata_no_of_rooms[room_choice] - 1; //updates the number of rooms available
            room_receipt = stay_duration * roomdata_roomprice[room_choice]; //generates bill
            cout<<"\n A bill of Rs. "<<room_receipt<<" has been generated for Mr/Mrs "<<cust_name<<". \n It will be added to the Records. \nPayments will be taken at Check-out.";
            update_data(1); //room data replaced
            update_data(3); //customer data appended
        }
        void get_roomdata(){
            string line = "", tempstring = "";
            int n = 0;
            ifstream room;
            room.open("Room_Database.csv");
            if(room.fail()){
                cout<<"\n Login Failed due to file error. Please check the file and run the code again! ";
                exit(1);
            }
            else{
                while( getline(room, line)){
                    stringstream Inputstring(line);

                    getline(Inputstring, roomdata_roomtype[n], ',');
                    getline(Inputstring, tempstring, ',');
                    roomdata_roomprice[n] = atoi(tempstring.c_str());
                    tempstring = "";
                    getline(Inputstring, tempstring, ',');
                    roomdata_no_of_rooms[n] = atoi(tempstring.c_str());
                    n++;
                }
            }
            room.close();
        }
        void get_custdata(){
            string line = "", tempstring = "";
            int n = 0;
            ifstream customer;
            customer.open("Customer_Database.csv");
            if(customer.fail()){
                cout<<"\n Login Failed due to file error. Please check the file and run the code again! ";
                exit(1);
            }
            else{
                while( getline(customer, line)){
                    stringstream Inputstring(line);

                    getline(Inputstring, customerdata_name[n], ',');
                    getline(Inputstring, tempstring, ',');
                    customerdata_roomno[n] = atoi(tempstring.c_str());
                    tempstring = "";
                    getline(Inputstring, tempstring, ',');
                    customerdata_outstanding[n] = atoi(tempstring.c_str());
                    tempstring = "";
                    getline(Inputstring, tempstring, ',');
                    customerdata_roomtype[n] = atoi(tempstring.c_str());
                    n++;
                }
            }
            customer.close();
        }
        void check_out(){
            cout<<"\n ====== CHECK-OUT PORTAL ====== \n";
            cout<<"\n Customer Details: ";
            cout<<"\nFull Name: ";
            cin.ignore(); //to clear any whitespace from the console.
            getline(cin, cust_name);
            verify_cust_data(1); //verifies if the customer has checked-in or not.
            if (verification_status == 0){
                cout<<"\n Customer not found in Records! ";
                return;
            }
            cout<<"\nRoom No.: ";
            cin>>room_no;
            verify_cust_data(2);
            if(verification_status == 0){
                cout<<"\n Incorrect Room No. Try Again. ";
                return;
            }
            cout<<"\n Outstanding Payment: Rs. "<<room_receipt;
            //re-direct to payment function
            cout<<"\n Updating Records.";
            //get_roomdata(); //for some reason getting stuck here
            roomdata_no_of_rooms[0] = roomdata_no_of_rooms[0] + 1; //updating Room Database
            update_data(1);
            update_data(2);

        }
        void verify_cust_data(int verification_type){
            get_custdata();
            verification_status = 0;
            if(verification_type == 1){ //check-in verification
                for(int i = 0; i<51; i++){
                    if(customerdata_name[i] == cust_name){
                        verification_status = 1; return;
                    }
                }
            }
            if(verification_type == 2){ //check-out verification
                for(int i = 0; i<51; i++){
                    if(customerdata_name[i] == cust_name && customerdata_roomno[i] == room_no){
                        verification_status = 1; customerindex = i;
                        room_choice = customerdata_roomtype[i]; //the room type is stored for updating room records
                        return;
                    }
                }
            }
        }
        void update_data(int update_type) {
            if (update_type == 1) {
                // Update room database
                ofstream room("Room_Database.csv");
                if (!room) {
                    cout << "Error: Failed to open Room_Database.csv for writing!" << endl;
                    exit(1);
                }
                for (int i = 0; i < types; i++) {
                    room << roomdata_roomtype[i] << ", " << roomdata_roomprice[i] << ", " << roomdata_no_of_rooms[i] << "\n";
                }
                room.close();
            } else if (update_type == 3) {
                // Append to customer database
                ofstream customer("Customer_Database.csv", ios::app);
                if (!customer) {
                    cout << "Error: Failed to open Customer_Database.csv for writing!" << endl;
                    exit(1);
                }
                customer << cust_name << ", " << room_no << ", " << room_receipt << ", " << room_choice << ", " << mobile_no << "\n";
                customer.close();
            } else if (update_type == 2) {
                // Remove customer from customer database
                ifstream infile("Customer_Database.csv");
                ofstream outfile("temp.csv");
                if (!infile || !outfile) {
                    cout << "Error: Failed to open files for processing!" << endl;
                    exit(1);
                }

                string line;
                int count = 0;
                while (getline(infile, line)) {
                    count++;
                    if (count != customerindex) {
                        outfile << line << endl;
                    }
                }

                infile.close();
                outfile.close();

                // Remove original file and rename temp file
                remove("Customer_Database.csv");
                if (rename("temp.csv", "Customer_Database.csv") != 0) {
                    cout << "Error: Failed to rename temp.csv to Customer_Database.csv!" << endl;
                    exit(1);
                }
            }
        }
        void payment();
};
class restaurant{
    string cust_name;
    int room_no, verification_status = 0;
    string menudata_itemname[54];
    int menudata_price[54];
    int menu_choice, customerindex;
    int rest_receipt = 0;
    //customer database variables
    string customerdata_name[51];
    int customerdata_roomno[51] = {0}, customerdata_roomtype[51];
    int customerdata_outstanding[51] = {0};
    public:
        restaurant(){
            string line = "", tempstring = "";
            int n = 0;
            ifstream customer;
            customer.open("Customer_Database.csv");
            if(customer.fail()){
                cout<<"\n Login Failed due to file error. Please check the file and run the code again! ";
                exit(1);
            }
            else{
                while(getline(customer, line)){
                    stringstream Inputstring(line);

                    getline(Inputstring, customerdata_name[n], ',');
                    getline(Inputstring, tempstring, ',');
                    customerdata_roomno[n] = atoi(tempstring.c_str());
                    tempstring = "";
                    getline(Inputstring, tempstring, ',');
                    customerdata_outstanding[n] = atoi(tempstring.c_str());
                    tempstring = "";
                    getline(Inputstring, tempstring, ',');
                    customerdata_roomtype[n] = atoi(tempstring.c_str());
                    n++;
                }
            }
            customer.close();
        }
        void display_menu(){
            string line = "", tempstring = ""; int n = 0;
            ifstream menu;
            menu.open("Restaurant_Database.csv");
            if(menu.fail()){
                cout<<"\n Login Failed due to file error. Please check the file and run the code again! ";
                exit(1);
            }
            else{
                while( getline(menu, line)){
                    stringstream Inputstring(line);

                    getline(Inputstring, menudata_itemname[n], ',');
                    getline(Inputstring, tempstring, ',');
                    menudata_price[n] = atoi(tempstring.c_str());
                    n++;
                }
            }
            menu.close();
            cout<<"\n ====== MENU ====== \n";
            cout<<"Item No. | Item Name | Price\n";
            for(int i = 0; i < 54; i++){
                cout<<i+1<<". \t"<<menudata_itemname[i]<<" \t - "<<menudata_price[i]<<endl;
            }
        }
        void verify_cust_data(){
            for(int i = 0; i<51; i++){
                    if(customerdata_name[i] == cust_name && customerdata_roomno[i] == room_no){
                        verification_status = 1; customerindex = i;
                        return;
                    }
                }
        }
        void take_order(){
            cout<<"====== RESTAURANT ======";
            cout<<"\n \nCustomer Full Name: ";
            cin.ignore(); //to clear any whitespace from the console.
            getline(cin, cust_name);
            cout<<"\n Enter Room No.: ";
            cin>>room_no;
            verification_status = 1; //bypassing
            if (verification_status == 0){ //not working
                cout<<"\n Customer name or Room No. Incorrect. Please Try Again.";
                return;
            }
            display_menu();
            rest_receipt = 0;
            do{
                cout<<"\n Order (item no.): ";
                cin>>menu_choice;
                menu_choice = valid(1, 54, menu_choice);
                rest_receipt = rest_receipt + menudata_price[menu_choice];
                cout<<"\n Press: \nAny number: Another Order | 0: Proceed to Billing ";
                cin>>menu_choice;
            }while (menu_choice != 0);
        cout<<"\n Restaurant Bill: Rs."<<rest_receipt;
        cout<<"\n Will be added to the Records and final payment will be recieved at Check-Out.";
        update_cust_data();
        }
        void update_cust_data(){
            ifstream infile("Customer_Database.csv");
            ofstream outfile("temp.csv");
            if (!infile || !outfile) {
                cout << "Error: Failed to open files for processing!" << endl;
                exit(1);
            }

            string line;
            while (getline(infile, line)) {
                string name, roomno_str, outstanding_str, roomtype_str;
                stringstream ss(line);
                getline(ss, name, ',');
                getline(ss, roomno_str, ',');
                getline(ss, outstanding_str, ',');
                getline(ss, roomtype_str, ',');
                
                // Convert room number and outstanding payment to integers
                int roomno = stoi(roomno_str);
                int outstanding = stoi(outstanding_str);
                int roomtype = stoi(roomtype_str);
                
                // Update outstanding payment for the specified customer
                if (name == cust_name && roomno == room_no) {
                    outstanding += rest_receipt; // Add restaurant bill to outstanding payment
                }
                
                // Write the updated record to the temporary file
                outfile << name << ", " << roomno << ", " << outstanding << ", " << roomtype << "\n";
            }

            infile.close();
            outfile.close();

            // Replace original file with the temporary file
            remove("Customer_Database.csv");
            if (rename("temp.csv", "Customer_Database.csv") != 0) {
                cout << "Error: Failed to rename temp.csv to Customer_Database.csv!" << endl;
                exit(1);
            }
        }
};
int main()
{
    int main_choice, reception_choice;
    cout<<"\n ======== PENINSULA HOTEL's MANAGER PORTAL ========"<<endl;
    manager m;
    restaurant rest;
    reception r;
    m.login();
    main_menu:
    cout<<"\n \n \n ======= CHOOSE ======= \n1. Reception \n2. Restaurant \n3. Exit \nChoice: ";
    cin>>main_choice;
    switch (main_choice)
    {
        case 1:
            reception_menu:
            cout<<"\n \n ===== RECEPTION ===== \n1. Check-In \n2. Check-Out \n3. Back \nChoice: ";
            cin>>reception_choice;
            switch (reception_choice)
            {   
                case 1:
                    r.check_in();
                    goto reception_menu;
                case 2:
                    r.check_out();
                    goto reception_menu;
                case 3:
                    cout<<"\n Exiting Reception...";
                    goto main_menu;
                default:
                    cout<<"\n INVALID INPUT! \n";
                    goto reception_menu;
            }
            goto main_menu;
        case 2:
            rest.take_order();
            goto main_menu;
        case 3:
            cout<<"\n ========== EXITING CODE ==========";
            exit(1);
        default:
            cout<<"\n INVALID INPUT! \n";
            goto main_menu;  
    }
    return 0;
}
