//Melis Doğan 26661

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
using namespace std;

//Globals:
bool is_Plane_Full = false;
int count1 = 100;
int turn = 0;

//vector for plane seatings
vector<vector<int> > vector_of_plane(2,vector<int>(50));

void * Travel_Agency_Thread1(void * ) //Thread func for agency 1 for seat booking
{
  
  while(!is_Plane_Full){
      srand(time(NULL)); // randomization per execution
      while(!is_Plane_Full && turn==0){
          int row = rand() % 2;
          int col = rand() % 50;
          bool rez = false;
          if(!vector_of_plane[row][col]){ // if seat is not reserved
              cout << "Agency 1 Entered Critical Region"<< endl;
              count1--;
              int seat = (row*50 + col + 1); //seat number initializer
              cout << "Seat Number "<< seat <<" is reserved by Agency 1" <<endl;
              vector_of_plane[row][col] = 1; //filler of that specific place in vector
              rez = true; // reservation is now made by agency
              cout << "Agency 1 Exit Critical Region" <<endl;
              if (is_Plane_Full){ // warning when seats finish
                  cout<< "No Seats Left"<< endl;
              }
              cout << endl; }
          if(rez){
              turn = 1;
              
          }
      }
  }
    return 0;
}
void * Travel_Agency_Thread2(void * ) //Thread func for agency 2 for seat booking
{
  
    while(!is_Plane_Full){
      while(!is_Plane_Full && turn==1){
          int row = rand() % 2;
          int col = rand() % 50;
          bool rez = false;
          if(!vector_of_plane[row][col]){
              cout << "Agency 2 Entered Critical Region"<< endl;
              count1--;
              int seat = (row*50 + col + 1);
              cout << "Seat Number "<< seat <<" is reserved by Agency 2" <<endl;
              vector_of_plane[row][col] = 2;
              rez = true;
              cout << "Agency 2 Exit Critical Region" <<endl;
              if (is_Plane_Full){
                  cout<< "No Seats Left"<< endl;
              }
              cout << endl;
          
          }
          if(rez){
              turn = 0;
          }
      }
    }
    return 0;
}

int main()
{
  pthread_t thread1, thread2;
  int TravelAgency1 = 0, TravelAgency2 = 1;
  pthread_create( &thread1, NULL, Travel_Agency_Thread1, (void *) & TravelAgency1);//threads are now created
  pthread_create( &thread2, NULL, Travel_Agency_Thread2, (void *) & TravelAgency2);
  int counter = 0;
    while(!is_Plane_Full) //actual function that fills the vector of plane seating
    {
        for(int i=0;i < vector_of_plane.size(); i++)
        {
            for(int j=0;j < vector_of_plane[0].size(); j++)
            {
                if(vector_of_plane[i][j])
                    counter++;
            }
        }
        if(counter == 100)
            is_Plane_Full = true;
        else
            counter = 0;
    }
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    cout <<"Plane is full:"<< endl;
    for (size_t i = 0; i < 2; i++) { //Printing the seating of the plane
    cout << "Seat "<<i+1<<": ";
    for (size_t j = 0; j < 50; j++) {
      cout << vector_of_plane[i][j] << " ";
    }
      cout << endl;
  }
    return 0;
}

