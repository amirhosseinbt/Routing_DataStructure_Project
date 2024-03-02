
#include "../include/Besttime.hpp"

Besttime::Besttime()
{
    parents = new pair<pair<TimedPath,int>,pair<bool,int>>[N];
}
Besttime::~Besttime()
{
    delete[] parents;
}

int Besttime::getIndexFromParents(int i,int currentVertex)
{
    if (parents[currentVertex].second.second == i)
    {
        return currentVertex;
    }
    getIndexFromParents(i,parents[currentVertex].second.second);
}
void Besttime::FillAdjMatrix(std::vector<Station> * stations,std::vector<Path>* path)
{
    //filling the matrix with indices from stations
    //each adjMatrix[i][j] is a TimedPath object that filling from Distance.txt
    for (int i = 0; i < stations->size(); i++)
    {
        std::vector <TimedPath> row;
        for (int j = 0; j < stations->size(); j++)
        {
            TimedPath tempPath;
            if(i == j)
            {
                tempPath.setFirstST((*stations)[i].GetName());
                tempPath.setSecondST((*stations)[i].GetName());
                tempPath.setBusTime(0);
                tempPath.setSubwayTime(0);
                tempPath.setTaxiTime(0);
                row.push_back(tempPath);
            }
            else
            {
                TimedPath temp;
                Bus bus;
                Taxi taxi;
                Subway subway;
                //searching in path vector for matching the station's name
                for (int k = 0; k < path->size(); k++)
                {
                    if(((*path)[k].getFirstST()==(*stations)[i].GetName() && (*path)[k].getSecondST()==(*stations)[j].GetName())
                    ||((*path)[k].getFirstST()==(*stations)[j].GetName() && (*path)[k].getSecondST()==(*stations)[i].GetName()))
                    {
                        temp.setFirstST((*stations)[i].GetName());
                        temp.setSecondST((*stations)[j].GetName());
                        if((*path)[k].getBusDistance() >= 0 ||(*path)[k].getTrainTaxiDistance() >= 0)
                        {
                            temp.setBusTime(((*path)[k].getBusDistance())*bus.getMinute_per_km());
                            temp.setTaxiTime(((*path)[k].getTrainTaxiDistance())*taxi.getMinute_per_km());
                            temp.setSubwayTime(((*path)[k].getTrainTaxiDistance())*subway.getMinute_per_km());
                        }
                        else
                        {
                            temp.setBusTime(0);
                            temp.setTaxiTime(0);
                            temp.setSubwayTime(0);
                        }
                        temp.setSubway_Taxi_Line((*path)[k].getSubway_Taxi_Line());
                        temp.setBus_Line((*path)[k].getBus_Line());
                    }
                }
                row.push_back(temp);
            }    
        }
        adjMatrix.push_back(row);
    }
}
void Besttime::PrintAdjMatrix(std::vector <Station>* stations)
{
    for (int i = 0; i < adjMatrix.size(); i++)
    {
        std::cout << (*stations)[i].GetName() <<'\n';
        for (int j = 0; j < adjMatrix.size(); j++)
        {
            cout <<"firstST : "<<adjMatrix[i][j].getFirstST() << "  secondST : " << adjMatrix[i][j].getSecondST()
             << "   " << adjMatrix[i][j].getSubwayTime() << " , "<<adjMatrix[i][j].getTaxiTime()<<" , "<<
             adjMatrix[i][j].getBusTime()<<"     "<<adjMatrix[i][j].getSubway_Taxi_Line() 
             <<" , " << adjMatrix[i][j].getBus_Line()<<'\n';
        }
        std::cout << '\n';
    }
}
void Besttime::Dijkstra(int source)
{
    delete[] parents;
    parents = new pair<pair<TimedPath,int>,pair<bool,int>>[N];
    /*added[i] will true if vertex i is included in shortest path tree or shortest distance from src to
    i is finalized*/
    std::vector<bool> added(N);
    Time time_during_dijkstra = arriving_time;
    // Initialize all distances as
    // INFINITE and added[] as false
    for (int vertexIndex = 0; vertexIndex < N;
         vertexIndex++) {
        dijkstraList[vertexIndex] = INT_MAX;
        added[vertexIndex] = false;
    }
 
    // Distance of source vertex from
    // itself is always 0
    Bus bus;
    Taxi taxi;
    Subway subway;
    // if(vehicle == "bus")
    // {
    //     if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
    //         || (stoi(time_during_dijkstra.getHour()) == 8 
    //         && stoi(time_during_dijkstra.getMinute())==0))
    //     {
    //         parents[source].first.second = bus.getMinute_gettingOn_gettingOff_Traffic();
    //         time_during_dijkstra = time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic();
    //         parents[source].second.first = true;
    //     }
    //     else
    //     {
    //         parents[source].first.second = bus.getMinute_gettingOn_gettingOff();
    //         time_during_dijkstra = time_during_dijkstra + bus.getMinute_gettingOn_gettingOff();
    //         parents[source].second.first = false;
    //     }
    //     parents[source].first.first.setBusStatus(true);
    //     parents[source].first.first.setTaxiStatus(false);
    //     parents[source].first.first.setSubwayStatus(false);
        
    // }
    // else if(vehicle == "taxi")
    // {
    //     if (stoi(time_during_dijkstra.getHour()) >= 18 && stoi(time_during_dijkstra.getHour())<20
    //         || (stoi(time_during_dijkstra.getHour()) == 20 
    //         && stoi(time_during_dijkstra.getMinute())==0))
    //     {
    //         parents[source].first.second = taxi.getMinute_gettingOn_gettingOff_Traffic();
    //         time_during_dijkstra = time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic();
    //         parents[source].second.first = true;
    //     }
    //     else
    //     {
    //         parents[source].first.second = taxi.getMinute_gettingOn_gettingOff();
    //         time_during_dijkstra = time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff();
    //         parents[source].second.first = false;
    //     }
    //     parents[source].first.first.setBusStatus(false);
    //     parents[source].first.first.setTaxiStatus(true);
    //     parents[source].first.first.setSubwayStatus(false);
    // }
    // else if (vehicle == "subway")
    // {
    //     if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
    //         || (stoi(time_during_dijkstra.getHour()) == 8 
    //         && stoi(time_during_dijkstra.getMinute())==0))
    //     {
    //         parents[source].first.second = subway.getMinute_gettingOn_gettingOff_Traffic();
    //         time_during_dijkstra = time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic();
    //         parents[source].second.first = true;
    //     }
    //     else
    //     {
    //         parents[source].first.second = subway.getMinute_gettingOn_gettingOff();
    //         time_during_dijkstra = time_during_dijkstra + subway.getMinute_gettingOn_gettingOff();
    //         parents[source].second.first = false;
    //     }
    //     parents[source].first.first.setBusStatus(false);
    //     parents[source].first.first.setTaxiStatus(false);
    //     parents[source].first.first.setSubwayStatus(true);
    // }
    dijkstraList[source] = 0;
 
    // The starting vertex does not
    // have a parent
    parents[source].second.second = -1;
    // cout << "hi "<< vehicle << '\n';
    // Find shortest path for all
    // vertices
    for (int i = 1; i < N; i++) {
        /*Pick the minimum distance vertex from the set of vertices not yet processed.
        nearestVertex is always equal to startNode in first iteration.*/
        // cout << i << " i"<< '\n';
        int nearestVertex = -1;
        int shortestTime = INT_MAX;
        for (int vertexIndex = 0; vertexIndex < N;
             vertexIndex++) {
            if (!added[vertexIndex] && dijkstraList[vertexIndex] <shortestTime) 
            {
                nearestVertex = vertexIndex;
                shortestTime = dijkstraList[vertexIndex];
            }
        }
        /* Mark the picked vertex as processed*/
        added[nearestVertex] = true;
 
        /* Update dist value of the adjacent vertices of the picked vertex.*/ 
        for (int vertexIndex = 0; vertexIndex < N; vertexIndex++) 
        {
            int weight=0;
            bool flag = false; // check existing path
            bool by_bus = false;
            bool by_subway = false;
            bool by_taxi = false;
            bool traffic_time = false;
            std::string line="";
            int busTime =adjMatrix[nearestVertex][vertexIndex].getBusTime();
            int taxiTime =adjMatrix[nearestVertex][vertexIndex].getTaxiTime();
            int subwayTime =adjMatrix[nearestVertex][vertexIndex].getSubwayTime();

            if(nearestVertex == source)
            {
                // cout << busTime << " bus Time " << '\n';
                // cout << taxiTime << " taxi time " << '\n';         
                // cout << subwayTime << " subway time " << '\n';         
                // if (vehicle == "bus")
                // {
                    if(busTime != 0)
                    {
                        // busTime = busTime + parents[source].first.second;
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                traffic_time = true;
                                
                            }
                            else 
                            {
                                traffic_time = false;
                            }
                            busTime += bus.getMinute_gettingOn_gettingOff_Traffic();
                        }
                        else
                        {
                            
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff()).getHour())<8
                            || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                busTime = busTime;
                                traffic_time  = false;
                            }
                            busTime += bus.getMinute_gettingOn_gettingOff();
                        }
                        
                    }
                // }
                // else if (vehicle == "subway")
                // {
                    if(subwayTime != 0)
                    {
                        // subwayTime = subwayTime + parents[source].first.second;
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            subwayTime += subway.getMinute_gettingOn_gettingOff_Traffic();
                        }
                        else
                        {
                            
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff()).getHour())<8
                            || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time  = false;
                            }
                            subwayTime += subway.getMinute_gettingOn_gettingOff();
                        }
                        
                    }
                    
                // }
                // else if (vehicle == "taxi")
                // {
                    if(taxiTime != 0)
                    {
                        // taxiTime = taxiTime + parents[source].first.second;
                        if (stoi(time_during_dijkstra.getHour()) >= 18 && stoi(time_during_dijkstra.getHour())<20
                        || (stoi(time_during_dijkstra.getHour()) == 20 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 18 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour())<20
                            || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 20 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            taxiTime += taxi.getMinute_gettingOn_gettingOff_Traffic();
                        }
                        else
                        {
                            
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 18 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour())<20
                            || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 20 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                taxiTime = taxiTime;
                                traffic_time = false;

                            }
                            taxiTime += taxi.getMinute_gettingOn_gettingOff();
                        }
                    }
                    
                // }
                busTime = busTime != 0 ? busTime : INT_MAX;
                taxiTime = taxiTime != 0 ? taxiTime : INT_MAX;
                subwayTime = subwayTime != 0 ? subwayTime : INT_MAX;
                if(busTime != INT_MAX && busTime < taxiTime && busTime < subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = busTime;
                    flag= true;
                    by_bus = true;
                    by_taxi = false;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                }
                else if (taxiTime != INT_MAX && taxiTime < busTime && taxiTime < subwayTime)// equal conditions must be checked
                {
                    // cout << " in " << '\n';
                    weight = taxiTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = true;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else if (subwayTime != INT_MAX && subwayTime < busTime && subwayTime < taxiTime)
                {
                    // cout << " in " << '\n';
                    weight = subwayTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = false;
                    by_subway = true;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else if (busTime != INT_MAX && busTime == taxiTime && busTime < subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = busTime;
                    flag= true;
                    by_bus = true;
                    by_taxi = false;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                }
                else if (busTime != INT_MAX && busTime < taxiTime && busTime == subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = busTime;
                    flag= true;
                    by_bus = true;
                    by_taxi = false;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                }
                else if (taxiTime != INT_MAX && taxiTime < busTime && taxiTime == subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = taxiTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = true;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else if (subwayTime != INT_MAX && subwayTime == busTime && subwayTime == taxiTime)
                {
                    // cout << " in " << '\n';
                    weight = subwayTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = false;
                    by_subway = true;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else
                {
                    flag = false;
                }
            }
            else
            {
                time_during_dijkstra = arriving_time + shortestTime;
                busTime = busTime != 0 ? busTime : INT_MAX;
                taxiTime = taxiTime != 0 ? taxiTime : INT_MAX;
                subwayTime = subwayTime != 0 ? subwayTime : INT_MAX;
                if(busTime != INT_MAX)
                {
                    
                    if(parents[nearestVertex].first.first.getBusStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getBus_Line() == parents[nearestVertex].first.first.getBus_Line())
                    {
                        if(parents[nearestVertex].second.first == true)
                        {
                            busTime = busTime * bus_time_coefficient_traffic;
                            traffic_time = true;
                        }
                        else
                        {
                            traffic_time = false;
                            busTime = busTime;
                        }
                    }
                    else if (adjMatrix[nearestVertex][vertexIndex].getBus_Line() != parents[nearestVertex].first.first.getBus_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            busTime = busTime + bus.getMinute_gettingOn_gettingOff_Traffic();

                            
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour())<8
                        || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                        && stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                busTime = busTime + bus.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                busTime = busTime + bus.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }                        
                        
                    }
                    else
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            busTime = busTime + bus.getMinute_gettingOn_gettingOff_Traffic();

                            
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour())<8
                        || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                        && stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                busTime = busTime + bus.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                busTime = busTime + bus.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }  
                    }
                }
                if (subwayTime != INT_MAX)
                {
                    if(parents[nearestVertex].first.first.getSubwayStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() == parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if(parents[nearestVertex].second.first == true)
                        {
                            subwayTime = subwayTime;
                            traffic_time = true;
                        }
                        else
                        {
                            traffic_time = false;
                            subwayTime = subwayTime;
                        }
                    }
                    else if(parents[nearestVertex].first.first.getTaxiStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() == parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff_Traffic();
                            
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour())<8
                        || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                        && stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }                        
                    }
                    else if (adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() != parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                traffic_time = true;
                            }
                            else 
                            {
                                traffic_time = false;
                            }
                            
                            subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff_Traffic();
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour())<8
                        || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                        && stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }                        
                        
                    }
                }
                if(taxiTime != INT_MAX)
                {
                    
                    if(parents[nearestVertex].first.first.getTaxiStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() == parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if(parents[nearestVertex].second.first == true)
                        {
                            taxiTime = taxiTime * taxi_time_coefficient_traffic;
                            traffic_time = true;
                        }
                        else
                        {
                            traffic_time = false;
                            taxiTime = taxiTime;
                        }
                    }
                    else if(parents[nearestVertex].first.first.getSubwayStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() == parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 18 && stoi(time_during_dijkstra.getHour())<20
                        || (stoi(time_during_dijkstra.getHour()) == 20 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 18 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour())<20
                            || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 20 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                traffic_time = true;
                                
                            }
                            else 
                            {
                                traffic_time = false;
                            }
                            taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff_Traffic();
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour()) >= 18 
                            && stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour())<20
                        || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour()) == 20 
                        && stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }  
                    }
                    else if (adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() != 
                    parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 18 && stoi(time_during_dijkstra.getHour())<20
                        || (stoi(time_during_dijkstra.getHour()) == 20 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 18 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour())<20
                            || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 20 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            
                            taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff_Traffic();
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour()) >= 18 
                            && stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour())<20
                        || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour()) == 20 
                        && stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }                        
                        
                    }
                }
                // if (nearestVertex == 0)
                // {
                    /* code */
                //     cout << busTime << ' ' << taxiTime << ' ' << subwayTime << '\n';
                // }
                
                if(busTime != INT_MAX && busTime < taxiTime && busTime < subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = busTime;
                    flag= true;
                    by_bus = true;
                    by_taxi = false;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                }
                else if (taxiTime != INT_MAX && taxiTime < busTime && taxiTime < subwayTime)// equal conditions must be checked
                {
                    // cout << " in " << '\n';
                    weight = taxiTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = true;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else if (subwayTime != INT_MAX && subwayTime < busTime && subwayTime < taxiTime)
                {
                    // cout << " in " << '\n';
                    weight = subwayTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = false;
                    by_subway = true;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else if (busTime != INT_MAX && busTime == taxiTime && busTime < subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = busTime;
                    flag= true;
                    by_bus = true;
                    by_taxi = false;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                }
                else if (busTime != INT_MAX && busTime < taxiTime && busTime == subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = busTime;
                    flag= true;
                    by_bus = true;
                    by_taxi = false;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                }
                else if (taxiTime != INT_MAX && taxiTime < busTime && taxiTime == subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = taxiTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = true;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else if (subwayTime != INT_MAX && subwayTime == busTime && subwayTime == taxiTime)
                {
                    // cout << " in " << '\n';
                    weight = subwayTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = false;
                    by_subway = true;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else
                {
                    flag = false;
                }
                
            }
            if (!added[vertexIndex]&& dijkstraList[nearestVertex] != INT_MAX && flag
             && ((shortestTime + weight) < dijkstraList[vertexIndex])) 
            {
                // cout << " after " << '\n';
                parents[vertexIndex].first.second = weight;
                parents[vertexIndex].second.first = traffic_time;
                parents[vertexIndex].second.second=nearestVertex;
                parents[vertexIndex].first.first.setBusStatus(by_bus);
                parents[vertexIndex].first.first.setTaxiStatus(by_taxi);
                parents[vertexIndex].first.first.setSubwayStatus(by_subway);
                if(by_bus)
                {
                    parents[vertexIndex].first.first.setBus_Line(line);
                    parents[vertexIndex].first.first.setSubway_Taxi_Line("");
                }
                else if (by_taxi || by_subway)
                {
                    parents[vertexIndex].first.first.setBus_Line("");
                    parents[vertexIndex].first.first.setSubway_Taxi_Line(line);
                }
                dijkstraList[vertexIndex] = shortestTime + weight;
            }
        }
    }
}

void Besttime::Dijkstra(int source,string vehicle)
{
    delete[] parents;
    parents = new pair<pair<TimedPath,int>,pair<bool,int>>[N];
    /*added[i] will true if vertex i is included in shortest path tree or shortest distance from src to
    i is finalized*/
    std::vector<bool> added(N);
    Time time_during_dijkstra = arriving_time;
    // Initialize all distances as
    // INFINITE and added[] as false
    for (int vertexIndex = 0; vertexIndex < N;
         vertexIndex++) {
        dijkstraList[vertexIndex] = INT_MAX;
        added[vertexIndex] = false;
    }
 
    // Distance of source vertex from
    // itself is always 0
    Bus bus;
    Taxi taxi;
    Subway subway;
    if(vehicle == "bus")
    {
        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
            || (stoi(time_during_dijkstra.getHour()) == 8 
            && stoi(time_during_dijkstra.getMinute())==0))
        {
            parents[source].first.second = bus.getMinute_gettingOn_gettingOff_Traffic();
            time_during_dijkstra = time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic();
            parents[source].second.first = true;
        }
        else
        {
            parents[source].first.second = bus.getMinute_gettingOn_gettingOff();
            time_during_dijkstra = time_during_dijkstra + bus.getMinute_gettingOn_gettingOff();
            parents[source].second.first = false;
        }
        parents[source].first.first.setBusStatus(true);
        parents[source].first.first.setTaxiStatus(false);
        parents[source].first.first.setSubwayStatus(false);
        
    }
    else if(vehicle == "taxi")
    {
        if (stoi(time_during_dijkstra.getHour()) >= 18 && stoi(time_during_dijkstra.getHour())<20
            || (stoi(time_during_dijkstra.getHour()) == 20 
            && stoi(time_during_dijkstra.getMinute())==0))
        {
            parents[source].first.second = taxi.getMinute_gettingOn_gettingOff_Traffic();
            time_during_dijkstra = time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic();
            parents[source].second.first = true;
        }
        else
        {
            parents[source].first.second = taxi.getMinute_gettingOn_gettingOff();
            time_during_dijkstra = time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff();
            parents[source].second.first = false;
        }
        parents[source].first.first.setBusStatus(false);
        parents[source].first.first.setTaxiStatus(true);
        parents[source].first.first.setSubwayStatus(false);
    }
    else if (vehicle == "subway")
    {
        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
            || (stoi(time_during_dijkstra.getHour()) == 8 
            && stoi(time_during_dijkstra.getMinute())==0))
        {
            parents[source].first.second = subway.getMinute_gettingOn_gettingOff_Traffic();
            time_during_dijkstra = time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic();
            parents[source].second.first = true;
        }
        else
        {
            parents[source].first.second = subway.getMinute_gettingOn_gettingOff();
            time_during_dijkstra = time_during_dijkstra + subway.getMinute_gettingOn_gettingOff();
            parents[source].second.first = false;
        }
        parents[source].first.first.setBusStatus(false);
        parents[source].first.first.setTaxiStatus(false);
        parents[source].first.first.setSubwayStatus(true);
    }
    dijkstraList[source] = 0;
 
    // The starting vertex does not
    // have a parent
    parents[source].second.second = -1;
    // cout << "hi "<< vehicle << '\n';
    // Find shortest path for all
    // vertices
    for (int i = 1; i < N; i++) {
        /*Pick the minimum distance vertex from the set of vertices not yet processed.
        nearestVertex is always equal to startNode in first iteration.*/
        // cout << i << " i"<< '\n';
        int nearestVertex = -1;
        int shortestTime = INT_MAX;
        for (int vertexIndex = 0; vertexIndex < N;
             vertexIndex++) {
            if (!added[vertexIndex] && dijkstraList[vertexIndex] <shortestTime) 
            {
                nearestVertex = vertexIndex;
                shortestTime = dijkstraList[vertexIndex];
            }
        }
        /* Mark the picked vertex as processed*/
        added[nearestVertex] = true;
 
        /* Update dist value of the adjacent vertices of the picked vertex.*/ 
        for (int vertexIndex = 0; vertexIndex < N; vertexIndex++) 
        {
            int weight=0;
            bool flag = false; // check existing path
            bool by_bus = false;
            bool by_subway = false;
            bool by_taxi = false;
            bool traffic_time = false;
            std::string line="";
            int busTime =adjMatrix[nearestVertex][vertexIndex].getBusTime();
            int taxiTime =adjMatrix[nearestVertex][vertexIndex].getTaxiTime();
            int subwayTime =adjMatrix[nearestVertex][vertexIndex].getSubwayTime();

            if(nearestVertex == source)
            {
                // cout << busTime << " bus Time " << '\n';
                // cout << taxiTime << " taxi time " << '\n';         
                // cout << subwayTime << " subway time " << '\n';         
                if (vehicle == "bus")
                {
                    if(busTime != 0)
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                traffic_time = true;
                                
                            }
                            else 
                            {
                                traffic_time = false;
                            }
                            busTime = busTime + parents[source].first.second;
                            
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff()).getHour())<8
                            || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                busTime = busTime;
                                traffic_time  = false;
                            }
                            busTime = busTime + parents[source].first.second;
                        }
                        weight = busTime;
                        flag= true;
                        by_bus = true;
                        by_taxi = false;
                        by_subway = false;
                        line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                    }
                    else
                    {
                        flag = false;
                    }
                }
                else if (vehicle == "subway")
                {
                    if(subwayTime != 0)
                    {
                        
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            subwayTime = subwayTime + parents[source].first.second;
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff()).getHour())<8
                            || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time  = false;
                            }
                            subwayTime = subwayTime + parents[source].first.second;
                        }
                        weight = subwayTime;
                        flag= true;
                        by_bus = false;
                        by_taxi = false;
                        by_subway = true;
                        line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                    }
                    else
                    {
                        flag = false;
                    }
                }
                else if (vehicle == "taxi")
                {
                    if(taxiTime != 0)
                    {
                        
                        if (stoi(time_during_dijkstra.getHour()) >= 18 && stoi(time_during_dijkstra.getHour())<20
                        || (stoi(time_during_dijkstra.getHour()) == 20 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 18 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour())<20
                            || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 20 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            taxiTime = taxiTime + parents[source].first.second;
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 18 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour())<20
                            || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 20 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                taxiTime = taxiTime;
                                traffic_time = false;

                            }
                            taxiTime = taxiTime + parents[source].first.second;
                        }
                        weight = taxiTime;
                        flag= true;
                        by_bus = false;
                        by_taxi = true;
                        by_subway = false;
                        line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                    }
                    else
                    {
                        flag = false;
                    }
                }
                
            }
            else
            {
                time_during_dijkstra = arriving_time + shortestTime;
                busTime = busTime != 0 ? busTime : INT_MAX;
                taxiTime = taxiTime != 0 ? taxiTime : INT_MAX;
                subwayTime = subwayTime != 0 ? subwayTime : INT_MAX;
                if(busTime != INT_MAX)
                {
                    
                    if(parents[nearestVertex].first.first.getBusStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getBus_Line() == parents[nearestVertex].first.first.getBus_Line())
                    {
                        if(parents[nearestVertex].second.first == true)
                        {
                            busTime = busTime * bus_time_coefficient_traffic;
                            traffic_time = true;
                        }
                        else
                        {
                            traffic_time = false;
                            busTime = busTime;
                        }
                    }
                    else if (adjMatrix[nearestVertex][vertexIndex].getBus_Line() != parents[nearestVertex].first.first.getBus_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            busTime = busTime + bus.getMinute_gettingOn_gettingOff_Traffic();

                            
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour())<8
                        || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                        && stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                busTime = busTime + bus.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                busTime = busTime + bus.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }                        
                        
                    }
                    else
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+bus.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            busTime = busTime + bus.getMinute_gettingOn_gettingOff_Traffic();

                            
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour())<8
                        || (stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                        && stoi((time_during_dijkstra + bus.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                busTime = busTime * bus_time_coefficient_traffic;
                                busTime = busTime + bus.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                busTime = busTime + bus.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }  
                    }
                }
                if (subwayTime != INT_MAX)
                {
                    if(parents[nearestVertex].first.first.getSubwayStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() == parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if(parents[nearestVertex].second.first == true)
                        {
                            subwayTime = subwayTime;
                            traffic_time = true;
                        }
                        else
                        {
                            traffic_time = false;
                            subwayTime = subwayTime;
                        }
                    }
                    else if(parents[nearestVertex].first.first.getTaxiStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() == parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff_Traffic();
                            
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour())<8
                        || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                        && stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }                        
                    }
                    else if (adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() != parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 6 && stoi(time_during_dijkstra.getHour())<8
                        || (stoi(time_during_dijkstra.getHour()) == 8 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 6 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getHour())<8
                            || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 8 
                            && stoi((time_during_dijkstra+subway.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                traffic_time = true;
                            }
                            else 
                            {
                                traffic_time = false;
                            }
                            
                            subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff_Traffic();
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) >= 6 
                            && stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour())<8
                        || (stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getHour()) == 8 
                        && stoi((time_during_dijkstra + subway.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                subwayTime = subwayTime + subway.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }                        
                        
                    }
                }
                if(taxiTime != INT_MAX)
                {
                    
                    if(parents[nearestVertex].first.first.getTaxiStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() == parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if(parents[nearestVertex].second.first == true)
                        {
                            taxiTime = taxiTime * taxi_time_coefficient_traffic;
                            traffic_time = true;
                        }
                        else
                        {
                            traffic_time = false;
                            taxiTime = taxiTime;
                        }
                    }
                    else if(parents[nearestVertex].first.first.getSubwayStatus()
                    && adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() == parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 18 && stoi(time_during_dijkstra.getHour())<20
                        || (stoi(time_during_dijkstra.getHour()) == 20 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 18 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour())<20
                            || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 20 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                traffic_time = true;
                                
                            }
                            else 
                            {
                                traffic_time = false;
                            }
                            taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff_Traffic();
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour()) >= 18 
                            && stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour())<20
                        || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour()) == 20 
                        && stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }  
                    }
                    else if (adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line() != 
                    parents[nearestVertex].first.first.getSubway_Taxi_Line())
                    {
                        if (stoi(time_during_dijkstra.getHour()) >= 18 && stoi(time_during_dijkstra.getHour())<20
                        || (stoi(time_during_dijkstra.getHour()) == 20 
                        && stoi(time_during_dijkstra.getMinute())==0))
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) >= 18 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour())<20
                            || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff_Traffic()).getHour()) == 20 
                            && stoi((time_during_dijkstra+taxi.getMinute_gettingOn_gettingOff_Traffic()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                traffic_time = true;
                            }
                            else
                            {
                                traffic_time = false;
                            }
                            
                            taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff_Traffic();
                        }
                        else
                        {
                            if (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour()) >= 18 
                            && stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour())<20
                        || (stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getHour()) == 20 
                        && stoi((time_during_dijkstra + taxi.getMinute_gettingOn_gettingOff()).getMinute())==0))
                            {
                                taxiTime = taxiTime * taxi_time_coefficient_traffic;
                                taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff();
                                traffic_time  = true;
                            }
                            else
                            {
                                taxiTime = taxiTime + taxi.getMinute_gettingOn_gettingOff();
                                traffic_time  = false;
                            }
                        }                        
                        
                    }
                }
                // if (nearestVertex == 0)
                // {
                    /* code */
                //     cout << busTime << ' ' << taxiTime << ' ' << subwayTime << '\n';
                // }
                
                if(busTime != INT_MAX && busTime < taxiTime && busTime < subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = busTime;
                    flag= true;
                    by_bus = true;
                    by_taxi = false;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                }
                else if (taxiTime != INT_MAX && taxiTime < busTime && taxiTime < subwayTime)// equal conditions must be checked
                {
                    // cout << " in " << '\n';
                    weight = taxiTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = true;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else if (subwayTime != INT_MAX && subwayTime < busTime && subwayTime < taxiTime)
                {
                    // cout << " in " << '\n';
                    weight = subwayTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = false;
                    by_subway = true;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else if (busTime != INT_MAX && busTime == taxiTime && busTime < subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = busTime;
                    flag= true;
                    by_bus = true;
                    by_taxi = false;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                }
                else if (busTime != INT_MAX && busTime < taxiTime && busTime == subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = busTime;
                    flag= true;
                    by_bus = true;
                    by_taxi = false;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getBus_Line();
                }
                else if (taxiTime != INT_MAX && taxiTime < busTime && taxiTime == subwayTime)
                {
                    // cout << " in " << '\n';
                    weight = taxiTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = true;
                    by_subway = false;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else if (subwayTime != INT_MAX && subwayTime == busTime && subwayTime == taxiTime)
                {
                    // cout << " in " << '\n';
                    weight = subwayTime;
                    flag = true;
                    by_bus = false;
                    by_taxi = false;
                    by_subway = true;
                    line = adjMatrix[nearestVertex][vertexIndex].getSubway_Taxi_Line();
                }
                else
                {
                    flag = false;
                }
                
            }
            if (!added[vertexIndex]&& dijkstraList[nearestVertex] != INT_MAX && flag
             && ((shortestTime + weight) < dijkstraList[vertexIndex])) 
            {
                // cout << " after " << '\n';
                parents[vertexIndex].first.second = weight;
                parents[vertexIndex].second.first = traffic_time;
                parents[vertexIndex].second.second=nearestVertex;
                parents[vertexIndex].first.first.setBusStatus(by_bus);
                parents[vertexIndex].first.first.setTaxiStatus(by_taxi);
                parents[vertexIndex].first.first.setSubwayStatus(by_subway);
                if(by_bus)
                {
                    parents[vertexIndex].first.first.setBus_Line(line);
                    parents[vertexIndex].first.first.setSubway_Taxi_Line("");
                }
                else if (by_taxi || by_subway)
                {
                    parents[vertexIndex].first.first.setBus_Line("");
                    parents[vertexIndex].first.first.setSubway_Taxi_Line(line);
                }
                dijkstraList[vertexIndex] = shortestTime + weight;
            }
        }
    }

}
void Besttime::printDijkstra(std::vector <Station>* stations)
{
    for (int i = 0; i < N; i++)
    {
        cout << (*stations)[i].GetName() <<"    " << dijkstraList[i]<<'\n';
    }
    
}
int* Besttime::getDijkstraList ()
{
    return dijkstraList;
}
pair<pair<TimedPath,int>,pair<bool,int>> * Besttime::getParents()
{
    return parents;
}
void Besttime::Print(std::vector<Station>* S ,int currentVertex,pair<pair<TimedPath,int>,pair<bool,int>>* parent)
{
    Bus bus;
    Subway subway;
    Taxi taxi;
    if (currentVertex == -1) {
        return;
    }

    
    Print(S,parent[currentVertex].second.second,parent);
    //below code gives path between origin and next station
    // if (parents[currentVertex].second.second == -1)
    // {
    //     if (parents[currentVertex].first.first.getBusStatus())
    //     {
    //         if (stoi(arriving_time.getHour()) >= 6 && stoi(arriving_time.getHour())<=8)
    //         {
    //             arriving_time = arriving_time + bus.getMinute_gettingOn_gettingOff_Traffic() ;
    //         }
    //         else
    //         {
    //             arriving_time = arriving_time + bus.getMinute_gettingOn_gettingOff();
    //         }
    //     }
    //     else if (parents[currentVertex].first.first.getSubwayStatus())
    //     {
    //         if (stoi(arriving_time.getHour()) >= 6 && stoi(arriving_time.getHour())<=8)
    //         {
    //             arriving_time = arriving_time + subway.getMinute_gettingOn_gettingOff_Traffic() ;
    //         }
    //         else
    //         {
    //             arriving_time = arriving_time + subway.getMinute_gettingOn_gettingOff();
    //         }
    //     }
    //     else if (parents[currentVertex].first.first.getTaxiStatus())
    //     {
    //         if (stoi(arriving_time.getHour()) >= 18 && stoi(arriving_time.getHour())<=20)
    //         {
    //             arriving_time = arriving_time + taxi.getMinute_gettingOn_gettingOff_Traffic() ;
    //         }
    //         else
    //         {
    //             arriving_time = arriving_time + taxi.getMinute_gettingOn_gettingOff();
    //         }
    //     }
        
    // }
    if (parent[currentVertex].second.second != -1) 
    {
        if (parent[currentVertex].first.first.getBusStatus())
        {
            cout<<(*S)[currentVertex].GetName() << " ( "
            <<parent[currentVertex].first.first.getBus_Line()<<" , Bus ) " << '\n';
            // arriving_time = arriving_time + parent[currentVertex].first.second;
        }
        else if (parent[currentVertex].first.first.getTaxiStatus())
        {
            cout<<(*S)[currentVertex].GetName() << " ( "
            <<parent[currentVertex].first.first.getSubway_Taxi_Line()<<" , Taxi ) "<<'\n';
            // arriving_time = arriving_time + parent[currentVertex].first.second;
        }
        else if (parent[currentVertex].first.first.getSubwayStatus())
        {
            cout<<(*S)[currentVertex].GetName() << " ( "
            <<parent[currentVertex].first.first.getSubway_Taxi_Line()<<" , Subway ) "<<'\n';
            // arriving_time = arriving_time + parent[currentVertex].first.second;
        }

    }
    
        // arriving_time = arriving_time + (includedStations[currentVertex].first.second * 1);

    // if(parents[currentVertex].first.first.getBusStatus())
    // {
        // cout<<(*S)[currentVertex].GetName() << " ( "
        // <<parents[currentVertex].first.first.getBus_Line()<<" , Bus ) " << '\n';
        // check changing in subway to bus
    //     if (parents[parents[currentVertex].second.second].first.first.getSubwayStatus())
    //     {
    //         if (stoi(arriving_time.getHour()) >= 6 && stoi(arriving_time.getHour())<=8)
    //         {
    //             arriving_time = arriving_time + bus.getMinute_gettingOn_gettingOff_Traffic() ;
    //         }
    //         else
    //         {
    //             arriving_time = arriving_time + bus.getMinute_gettingOn_gettingOff();
    //         }
            
    //     }
    //     else if (parents[parents[currentVertex].second.second].first.first.getTaxiStatus())
    //     {
    //         if (stoi(arriving_time.getHour()) >= 6 && stoi(arriving_time.getHour())<=8)
    //         {
    //             arriving_time = arriving_time + bus.getMinute_gettingOn_gettingOff_Traffic() ;
    //         }
    //         else
    //         {
    //             arriving_time = arriving_time + bus.getMinute_gettingOn_gettingOff();
    //         }
    //     }
    //     // check changing in one bus's line to another
    //     else if(parents[parents[currentVertex].second.second].first.first.getBus_Line() != ""
    //     &&(parents[parents[currentVertex].second.second].first.first.getBus_Line() != 
    //     parents[currentVertex].first.first.getBus_Line()))
    //     {
    //         if (stoi(arriving_time.getHour()) >= 6 && stoi(arriving_time.getHour())<=8)
    //         {
    //             arriving_time = arriving_time + bus.getMinute_gettingOn_gettingOff_Traffic();

    //         }
    //         else
    //         {
    //             arriving_time = arriving_time + bus.getMinute_gettingOn_gettingOff();
    //         }
    //     }

    //     arriving_time = arriving_time + parents[currentVertex].first.second;
    // }
    // else if(parents[currentVertex].first.first.getSubwayStatus())
    // {
    //     cout<<(*S)[currentVertex].GetName() << " ( "
    //     <<parents[currentVertex].first.first.getSubway_Taxi_Line()<<" , Subway ) "<<'\n';
    //     //check changing in bus to subway
    //     if (parents[parents[currentVertex].second.second].first.first.getBusStatus())
    //     {
    //         if (stoi(arriving_time.getHour()) >= 6 && stoi(arriving_time.getHour())<=8)
    //         {
    //             arriving_time = arriving_time + subway.getMinute_gettingOn_gettingOff_Traffic() ;
    //         }
    //         else
    //         {
    //             arriving_time = arriving_time + subway.getMinute_gettingOn_gettingOff() ;
    //         }
    //     }
    //     //check changing from taxi to subway
    //     else if (parents[parents[currentVertex].second.second].first.first.getTaxiStatus())
    //     {
    //         if (stoi(arriving_time.getHour()) >= 6 && stoi(arriving_time.getHour())<=8)
    //         {
    //             arriving_time = arriving_time + subway.getMinute_gettingOn_gettingOff_Traffic() ;
    //         }
    //         else
    //         {
    //             arriving_time = arriving_time + subway.getMinute_gettingOn_gettingOff() ;
    //         }
    //     }
    //     // check changing in one subway's line to another
    //     else if((parents[parents[currentVertex].second.second].first.first.getSubway_Taxi_Line() != 
    //     parents[currentVertex].first.first.getSubway_Taxi_Line()))
    //     {
    //         if (stoi(arriving_time.getHour()) >= 6 && stoi(arriving_time.getHour())<=8)
    //         {
    //             arriving_time = arriving_time + subway.getMinute_gettingOn_gettingOff_Traffic() ;
    //         }
    //         else
    //         {
    //             arriving_time = arriving_time + subway.getMinute_gettingOn_gettingOff() ;
    //         }
    //     }
    //     arriving_time = arriving_time + (includedStations[currentVertex].first.second * 1);
    // }
     
}
void Besttime::PrintPath(std::vector<Station> * S,int origin,int destination)
{
    for (int vertexIndex = 0; vertexIndex < N; vertexIndex++) 
    {
        if (vertexIndex != origin && vertexIndex == destination) {
            cout << "\n" << (*S)[origin].GetName() << " -> ";
            cout << (*S)[vertexIndex].GetName() << "   Best-Time : "  
            << dijkstraList[vertexIndex]<<" minutes" << '\n'<< '\n';
            Print(S,vertexIndex,parents);
            std::cout << '\n';
            cout << "Arriving Time : ";
            arriving_time.printTime();
        }
    }
}
void Besttime::setArrivingTime(Time t)
{
    arriving_time = t;
}
Time Besttime::getArrivingTime()
{
    return arriving_time;
}