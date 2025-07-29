#include <iostream>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;

class MaintenanceScheduler{
    private:
    
        string technician_code = "847392";
        
    public: 
        // Status and Logs
        int status = 1; // 0 -> MAINTENANCE, 1 -> ACTIVE
        int speed_arr[10] = {1500, 1500, 1490, 1500, 1496, 1500, 1490, 1496, 1485, 1500};
        int trip_counts = 50;
        int fault_count = 0;
        int total_trips = 0;
        int current_floor = 1;
        
        // Maintenance thresholds
        int max_trips = 2;
        int max_faults = 1;
        //int min_speed_threshold = 1450;
        int max_speed_threshold = 1500;

    public:
        // Speed monitoring function
        int speed_tracker(int speed){
            
            if(speed > max_speed_threshold){
                cout << "WARNING: Speed anomaly detected - " << speed << " units\n";
                fault_count++;
                return 0; // Fault detected
            }
            
            cout << "Speed normal: " << speed << " units\n";
            return 1; // Normal operation
        }

        // Time and trip tracking
        int time_tracker(){
            total_trips++;
            cout << "Trip completed. Total trips: " << total_trips << "\n";
            
            
            if(total_trips >= max_trips){
                cout << "MAINTENANCE REQUIRED: Trip limit reached (" << total_trips << "/" << max_trips << ")\n";
                return 0; 
            }
            
            return 1; 
        }

        // Verify technician access code
        bool verify_technician_code(){
            string entered_code;
            cout << "\n*** TECHNICIAN AUTHENTICATION REQUIRED ***\n";
            cout << "Enter 6-digit technician access code: ";
            cin >> entered_code;
            
            if(entered_code == technician_code){
                cout << "✓ Access granted. Technician authenticated.\n";
                return true;
            } else {
                cout << "✗ Access denied. Invalid technician code.\n";
                cout << "Unauthorized access attempt logged.\n";
                return false;
            }
        }

        // Technician maintenance stop
        int technician_stop(){
           
            if(!verify_technician_code()){
                return 0; 
            }
            
            cout << "\nTechnician initiated maintenance stop.\n";
            status = 0; 
            cout << "Elevator is now under maintenance.\n";
            cout << "Performing maintenance checks...\n";
            
            for(int i = 10; i > 0; i--){
                cout << "Maintenance in progress... " << i << " seconds remaining\n";
                this_thread::sleep_for(chrono::seconds(1));
            }
            
            // Reset counters after maintenance
            total_trips = 0;
            fault_count = 0;
            
            cout << "Maintenance completed. Elevator ready for service.\n";
            cout << "All fault counters reset. System operational.\n";
            status = 1;
            return 1;
        }

       
        bool needs_maintenance(){
            
            if(fault_count >= max_faults){
                cout << "MAINTENANCE REQUIRED: Too many faults (" << fault_count << "/" << max_faults << ")\n";
                return true;
            }
            
            
            if(total_trips >= max_trips){
                cout << "MAINTENANCE REQUIRED: Trip limit reached\n";
                return true;
            }
            
            return false;
        }

        // Simulate elevator movement
        void move_elevator(int dest_floor){
            if(status == 0){
                cout << "ERROR: Elevator is under maintenance. Cannot move.\n";
                return;
            }
            
            cout << "Moving from floor " << current_floor << " to floor " << dest_floor << "\n";
            
           
            int i = 0;
            while(current_floor != dest_floor){
                this_thread::sleep_for(chrono::seconds(1)); 

                int diff = abs(current_floor - dest_floor);
                
                
                int current_speed = speed_arr[i];
                
                if(current_floor < dest_floor){
                    current_floor++;
                    cout << "Moving to floor " << current_floor << "\n";
                } else {
                    current_floor--;
                    cout << "Moving to floor " << current_floor << "\n";
                }
                
                // Display status for each floor movement
                display_status();
                
               
                int speed_result = speed_tracker(current_speed);
                
                
                if(speed_result == 0){
                    cout << "\n*** FAULT DETECTED! ***\n";
                    cout << "Elevator movement stopped for safety evaluation.\n";
                    cout << "System entering emergency maintenance mode...\n";
                    status = 0; 
                    return;
                }
                
                i++;
                
               
                if(i >= 10) i = 0;
            }
            
            cout << "Arrived at floor " << current_floor << "\n";
            
          
            time_tracker();
            
           
            if(needs_maintenance()){
                status = 0; 
            }
        }

        // Emergency system shutdown (technician only)
        bool emergency_shutdown(){
            cout << "\n*** EMERGENCY SHUTDOWN REQUEST ***\n";
            cout << "This will immediately stop the elevator system.\n";
            
            if(!verify_technician_code()){
                return false; 
            }
            
            cout << "\n✓ Emergency shutdown authorized by technician.\n";
            cout << "System shutting down safely...\n";
            status = 0; 
            return true;
        }
        void display_status(){
            cout << "\n=== ELEVATOR STATUS ===" << endl;
            cout << "Status: " << (status ? "ACTIVE" : "MAINTENANCE") << endl;
            cout << "Current Floor: " << current_floor << endl;
            cout << "Total Trips: " << total_trips << "/" << max_trips << endl;
            cout << "Fault Count: " << fault_count << "/" << max_faults << endl;
            cout << "========================\n" << endl;
        }
};

int main(){
    MaintenanceScheduler car;
    string input;
    
    cout << "=== ELEVATOR MAINTENANCE SYSTEM ===" << endl;
    cout << "Commands: floor number, 'status', 'maintain', 'STOP'" << endl;
    
    while(true){
        car.display_status();
        this_thread::sleep_for(chrono::seconds(1)); 
        
        if(car.status == 0){
            cout << "The elevator is under maintenance." << endl;
            cout << "Enter 'maintain' to complete maintenance or 'STOP' to exit: ";
        } else {
            cout << "Enter destination floor (or 'status'/'maintain'/'STOP'): ";
        }
        
        cin >> input;
        
        // Check for STOP command
        if(input == "STOP"){
            if(car.emergency_shutdown()){
                cout << "System shutdown completed.\n";
                break;
            } else {
                cout << "Shutdown cancelled. System continues operation.\n";
                continue;
            }
        }
        
        // Handle maintenance command
        if(input == "maintain"){
            if(car.technician_stop() == 0){
                cout << "Maintenance access denied. System continues normal operation.\n";
            }
            continue;
        }
        
        // Handle status command
        if(input == "status"){
            car.display_status();
            continue;
        }
        
        // Handle floor number
        try {
            int dest_floor = stoi(input);
            
            if(dest_floor < 1 || dest_floor > 20){
                cout << "Invalid floor. Please enter floor 1-20." << endl;
                continue;
            }
            
            car.move_elevator(dest_floor);
            
        } catch(...) {
            cout << "Invalid input. Please enter a valid floor number or command." << endl;
        }
    }
    
    cout << "Final Status Report:" << endl;
    car.display_status();
    
    return 0;
}