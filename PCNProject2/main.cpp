//
//  main.cpp
//  PCNProject2
//
//  Created by Rahul R Dhanendran on 4/21/14.
//  Copyright (c) 2014 Rahul R Dhanendran. All rights reserved.
//

// Jason Jue

#include <iostream>
#include "rv.h"
#include "event.h"

double randomGenerator()
{
    double randn = ((double) rand() / (RAND_MAX));
    return randn;
}


int main()
{
    using namespace std;
    EventList Elist;                // Create event list
    enum {ARR,DEP1,DEP2,DEP3,TRANSFER11,TRANSFER13,TRANSFER23,TRANSFER32,TDEP1,TDEP2,TDEP3};                 // Define the event types
    enum{Q1,Q2,Q3};
    
    double lambda=1;            // Arrival rate
    double mu1 = 6.0;
    double mu2 = 25.0;
    double mu3 = 30.0;
    
        exp_rv(lambda);
    double rs1 = (double)1/4;
    double rs2 = (double)3/4;
    double r11 = (double)1/2,r13 = (double)1/2,r3d = (double)2/5,r32 =(double)3/5;
    
    int N = 0;                      // Number of customers in system
    int Ndep = 0;                   // Number of departures from system
    
                      // End condition satisfied?
    
    Event* CurrentEvent;
    
    
    for(lambda = 1; lambda <= 10 ; lambda++ )
    {
        
        int n1 = 0;
        int n2=0;
        int n3=0;
        int tn1 = 0;
        int tn2=0;
        int tn3=0;

        if(randomGenerator() <= rs1)
            Elist.insert(exp_rv(lambda),ARR,Q1);
        else
            Elist.insert(exp_rv(lambda),ARR,Q2);

        double clock = 0.0;             // System clock
        Ndep = 0;
        double EN1 = 0.0, EN2 = 0.0, EN3 = 0.0;
        int done = 0;
        while (!done)
        {
            CurrentEvent = Elist.get();
            double prev = clock;
            clock=CurrentEvent->time;
            
            
            
            switch (CurrentEvent->type)
            {
            case ARR:
                if(CurrentEvent->queue == 0)
                {
                    n1++;
                    //generate next arrival
                    if(randomGenerator() <= rs1)
                        Elist.insert(clock+exp_rv(lambda),ARR, Q1);
                    else
                        Elist.insert(clock+exp_rv(lambda),ARR, Q2);
                
                    if (n1==1)
                    {
                        Elist.insert(clock+exp_rv(mu1),DEP1, Q1);
                    }
                }
                else if(CurrentEvent->queue == 1)
                {
                    n2++;
                    //generate next arrival
                    if(randomGenerator() <= rs1)
                        Elist.insert(clock+exp_rv(lambda),ARR, Q1);
                    else
                        Elist.insert(clock+exp_rv(lambda),ARR, Q2);
                    
                    if (n2==1)
                    {
                        Elist.insert(clock+exp_rv(mu2),DEP2, Q2);
                    }
                }
                
                break;
                
                case DEP2:
                n2--;
                if (n2 > 0)
                {
                    Elist.insert(clock+exp_rv(mu2),DEP2,Q2);
                }
                
                Elist.insert(clock+exp_rv(mu2),TRANSFER23, Q3);
                break;
                
                
                
                break;
                
                
                case TRANSFER11:
                    tn1++;
                    if(tn1>0)
                    {
                        //Generate TDEP for this transfer event
                        Elist.insert(clock+exp_rv(mu1), TDEP1, Q1);
                    }
                    break;
                
                    
                case TDEP1:
                    tn1--;
                    if(randomGenerator()<=r11)
                    {
                    //Its going back to the same queue. Don't generate new arrival event. generate transfer event
                        Elist.insert(clock+exp_rv(mu1),TRANSFER11,Q1);
                    }
                    else
                    {
                        Elist.insert(clock+exp_rv(mu1),TRANSFER13,Q3);
                    
                    }
                break;
                
                    
                case TRANSFER13:
                    tn3++;
                    if(tn3>0)
                    {
                        //Generate TDEP for this transfer event
                        Elist.insert(clock+exp_rv(mu3), TDEP3, Q3);
                    }
                    break;
                    
                case TDEP3:
                    tn3--;
                    if(randomGenerator()<=r3d)
                    {
                        //Exits the system
                        Ndep++;
                    }
                    else
                    {
                        Elist.insert(clock+exp_rv(mu3),TRANSFER32,Q2);
                    }
                    break;
                
                case TRANSFER23:
                    tn3++;
                    if(tn3>0)
                    {
                        //Generate TDEP for this transfer event
                        Elist.insert(clock+exp_rv(mu3), TDEP3, Q3);
                    }
                    break;
                    
                    
                case TRANSFER32:
                    tn2++;
                    if(tn2>0)
                    {
                        //Generate TDEP for this transfer event
                        Elist.insert(clock+exp_rv(mu2), TDEP2, Q2);
                    }
                    break;
                    
                case TDEP2:
                    tn2--;
                    Elist.insert(clock+exp_rv(mu2),TRANSFER23, Q3);
                    break;
                
                case DEP1:
                    n1--;
                    if (n1 > 0)
                    {
                        Elist.insert(clock+exp_rv(mu1),DEP1,Q1);
                    }
                
                    
                    if(randomGenerator()<=r11)
                    {
                    //Its going back to the same queue. Don't generate new arrival event. generate transfer event
                        Elist.insert(clock+exp_rv(mu1),TRANSFER11,Q1);
                    
                    }
                    else
                    {
                        Elist.insert(clock+exp_rv(mu1),TRANSFER13,Q3);
                    }
                    break;
                
                
                
                
                
                }
            EN1 += (n1+tn1)*(clock-prev);
            EN2 += (n2+tn2)*(clock-prev);
            EN3 += (n3+tn3)*(clock-prev);
        
        
            //cout<<CurrentEvent->type<<endl;
            //cout<<"N1 : "<<n1+tn1<<"    N2 : "<<n2+tn2<<"    N3 : "<<n3+tn3<<"    Ndep : "<<Ndep<<endl;
            cout<<"EN1 : "<<EN1/clock<<"    EN2 : "<<EN2/clock<<"    EN3 : "<<EN3/clock<<"    Ndep : "<<Ndep<<endl;
        delete CurrentEvent;
        if (Ndep > 1000) done=1;        // End condition
    }
        Elist.clear();
        
}
    
}
