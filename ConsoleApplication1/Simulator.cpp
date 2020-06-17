#include "Simulator.h"

#include <ctime>
#include <array>
#include "Event.h"
#include "GeneratePacket.h"
#include "WirelessNetwork.h"
#include "ReceivePacket.h"
#include "FinishingSending.h"
#include "ScanConnection.h"
#include "Statistics.h"


void Simulator::add_and_sort_events(Event* event)
{
        eventList.push_back(event);
        eventList.sort([](const Event* event1, const Event* event2) //sorting by execute_time_ and event_priority_
        {
        	if(event1->execute_time_ == event2->execute_time_)
				return event1->event_priority_ < event2->event_priority_ ;

        	return event1->execute_time_ < event2->execute_time_;
        });
}


Simulator::Simulator(int mode, int text_mode, vector<double>seeds_vector, int number_of_simulation, double lambda, double probability, int K, int LR)
						: mode_(mode), text_mode_(text_mode), seeds_vector_(seeds_vector), number_of_simulation_(number_of_simulation), lambda_(lambda),
						probability_(probability),K_(K), LR_(LR), wireless_network_(new WirelessNetwork(seeds_vector, lambda, probability, this, K, LR))
{
}

void Simulator::Start(size_t time_of_simulation, size_t phase_time)
{
    this->phase_time = phase_time;
    //create Statistics object
    Statistics* statistics = new Statistics(wireless_network_->K, time_of_simulation, phase_time,  this);
    this->statistics = statistics;

	
    //save in file basic information about simulation
    string text = "SIMULATION NUMBER: ";
    text.append(to_string(number_of_simulation_)).append("\n\nLambda L = ").append(to_string(wireless_network_->lambda_)).append("\nProbability P = ").append(to_string(wireless_network_->P_)).append("\n\nKernels:\n");
    for (int i = 0; i < seeds_vector_.size(); i++)
    {
        text.append(to_string(seeds_vector_.at(i)).append("\n"));
    }
    text.append("\n\n");
    ofstream foutput;
    ifstream finput;
    string file_name = "logger_SIMULATION_";
    file_name.append(to_string(number_of_simulation_)).append(".txt");
    finput.open(file_name);
    foutput.open(file_name, ios::app);

    if (finput.is_open())
        foutput << text.append("\n");

    finput.close();
    foutput.close();

    text.clear();

	
    //with the start, program generate packets for each transmitters
    Simulator* simu = this;
    for (int i = 0; i < wireless_network_->K; i++)
    {
        //cout<<wireless_network_->transmitting_antennae.at(i)->generator_CGP->get_kernel()<<endl;

        GeneratePacket* generate_packet = new GeneratePacket((actual_time_ + (wireless_network_->transmitting_antennae.at(i)->generator_CGP->RandEXP(wireless_network_->lambda_))),
            i, wireless_network_, simu);
        add_and_sort_events(generate_packet);
    }

    Packet* pack_longest = new Packet(-1,-1,-1,-1, -1); //variable to release wireless_connection during packet collision

    bool set_acces_possible_to_false_after_while_loop = false; // <- it is use to change wireless_connection->set_acces_possible(false) to false, when some transmitter (or transmitters) gain access to wireless_connection

	
    while (actual_time_ < static_cast<unsigned int>(time_of_simulation))
    {
        srand(time(NULL));
        Logger logger;

        Event* ev = eventList.front();
        actual_time_ = ev->execute_time_;

        bool was_or_not = wireless_network_->wireless_connection->acces_possible1();// needful to check multi access to wireless_connection
    	
        ev->Execute();
        eventList.pop_front();


            //below condition is necessary, to allow all packets get wireless_connection in the same time 
            if (ev->event_priority_ == 3)
            {
                Event* ev2 = eventList.front();

                while(ev2->event_priority_ == 3 && ev2->execute_time_ == actual_time_)
                {
                    ev2->Execute();
                    eventList.pop_front();
                    ev2 = eventList.front();
                }
            }

            	
        bool no_event = false;

    	
        while (no_event == false)
        {
            no_event = true;

        	//Conditions for ScanConnection
            while(!scanConnectionQueue.empty()) {
                ScanConnection* first = scanConnectionQueue.front();

                if (first->wireless_network->wireless_connection->acces_possible1() == false)//if connectivity was scanned previous but now wireless_network is busy - MUST WAIT UNTIL WIRELESS_CONNECTION WILL BE FREE
                {
                    Logger logger;
                    first->how_many_free_ = 0;
                    string text = "Scanning connectivity for transmitter ";
                    logger.Info((text.append(to_string(first->trans_receiv_number_)).append(". Time of simulation: ").append(to_string(static_cast<double>(first->simulator->actual_time_)/2)).append(" [ms]. How many free: ").append(to_string(first->how_many_free_))), text_mode_, simu);

                    ScanConnection* sc = new ScanConnection(first->packet, (first->simulator->actual_time_ + first->wireless_network->S), first->how_many_free_, first->trans_receiv_number_, first->wireless_network, first->simulator);
                    first->simulator->add_and_sort_events(sc);
                    scanConnectionQueue.pop();
                    no_event = false;
                }
                if (first->wireless_network->wireless_connection->acces_possible1() == true && first->how_many_free_ == 2)//if connectivity is scanned 3th time and wireless_network is free - GAINED ACCESS! 
                {
                    Logger logger;
                    string text = "Gain access to connectivity for transmitter number ";
                    logger.Info(text.append(to_string(first->trans_receiv_number_)).append(". Time of simulation: ").append(to_string(static_cast<double>(first->simulator->actual_time_)/2)).append(" [ms]. How many free: ").append(to_string(first->how_many_free_ + 1)), text_mode_, simu);

                    //data to statistics
                    if(actual_time_ >= phase_time && first->packet->waiting_time_that_was_setted == false)
                    {
                        first->packet->waiting_time_that_was_setted = true;
                        statistics->add_average_waiting_time(first->packet->trans_rec_number, (actual_time_ - first->packet->start_life_time));
                    }
                	
                    first->wireless_network->wireless_connection->packets.push_back(first->packet);
                    set_acces_possible_to_false_after_while_loop = true;
                    //next instruction for Packet is executed by if's in main loop in Simulation.cpp
                    scanConnectionQueue.pop();
                    no_event = false;
                }
                if (first->wireless_network->wireless_connection->acces_possible1() == true && first->how_many_free_ < 2) //if connectivity was scanned previous and wireless_network was free - MUST HAVE A FREE CONNECTION 3 TIMES, TRY ON AGAIN 
                {
                    Logger logger;
                    first->how_many_free_++;

                    string text = "Scanning connectivity for transmitter ";
                    logger.Info(text.append(to_string(first->trans_receiv_number_)).append(". Time of simulation: ").append(to_string(static_cast<double>(first->simulator->actual_time_)/2)).append(" [ms]. How many free: ").append(to_string(first->how_many_free_)),text_mode_, simu);


                    ScanConnection* sc = new ScanConnection(first->packet, (first->simulator->actual_time_ + first->wireless_network->S), first->how_many_free_, first->trans_receiv_number_, first->wireless_network, first->simulator);
                    first->simulator->add_and_sort_events(sc);
                    scanConnectionQueue.pop();
                    no_event = false;
                }                               
            }
        	//condition change wireless_connection->set_acces_possible(false) flag to false, if in time of simulation at least one transmitter gain access to wireless_connection
        	if(set_acces_possible_to_false_after_while_loop == true)
        	{
                wireless_network_->wireless_connection->set_acces_possible(false);
                set_acces_possible_to_false_after_while_loop = false;
        	}


            //check that some transmitter gained access to wireless_connection (check that some packets are in a wireless_connection->packets)
            if (wireless_network_->wireless_connection->packets.size() == 1 && wireless_network_->wireless_connection->acces_possible1() == false)//if in wireless_connection->packets is only 1 packet - IS OK!  :)
            {                
                Simulator* simulator = this;
                ReceivePacket* rp = new ReceivePacket((wireless_network_->wireless_connection->packets.front()->time_of_service + actual_time_), simulator,
                    wireless_network_->wireless_connection->get_front_packet_from_wire_connection(), wireless_network_, false);
                add_and_sort_events(rp);

                //pack_longest = wireless_network_->wireless_connection->packets.front();
                wireless_network_->wireless_connection->delete_all_packets_from_wire_connection();                

                no_event = false;
            }
            if (wireless_network_->wireless_connection->packets.size() > 1 && wireless_network_->wireless_connection->acces_possible1() == false)//if in wireless_connection->packets is more than 1 packet - IS BAD!  :(
            {
                logger.Error("In wireless_connection is more than 1 packet! ", text_mode_, simu);

                for (Packet* packet : wireless_network_->wireless_connection->packets)
                {
                    string trans = "Transmitter ";
                    logger.Info(trans.append(to_string(packet->trans_rec_number).append(" gain access to wireless connectivity")),text_mode_, simu);

                    Simulator* simulator = this;
                    ReceivePacket* rp = new ReceivePacket(packet->time_of_service + actual_time_, simulator, packet, wireless_network_, true);
                    add_and_sort_events(rp);

                	
                    if(pack_longest->time_of_service <= packet->time_of_service) { pack_longest = packet; } //looking for packet with the longest execute_time <- is needed to unlock connectivity after the longest time among from packets
                	
                }
                wireless_network_->wireless_connection->delete_all_packets_from_wire_connection();

                no_event = false;
            }


            //check all ReceivingAntenna, that any contain Packet to process
            for (ReceivingAntenna* receiving_antenna : wireless_network_->receiving_antennae)
            {
                if (receiving_antenna->packet_ != nullptr)
                {
                    int ter = wireless_network_->generator_TER->RandZeroOne(wireless_network_->P_);

                    if (!ter && receiving_antenna->was_collision_ == false)
                    {
                        string text = "Transmission OK. Sending ACK info for transmitter ";
                        logger.Info(text.append(to_string(receiving_antenna->packet_->trans_rec_number)).append(". Simulation time: ").append(to_string(static_cast<double>(this->actual_time_)/2)).append(" [ms]."),text_mode_, simu);

                        FinishingSending* fs = new FinishingSending(this->actual_time_ + wireless_network_->CTIZ, receiving_antenna->packet_, wireless_network_, this);
                        add_and_sort_events(fs);
                    	//transmission was OK, it means receiver can send an ACK notification
                        no_event = false;

                        //data to statistics -> add_retransmissions_corrected_finished(...)
                        if (phase_time<=actual_time_)
                        {
                            statistics->add_correct_sent_packets(receiving_antenna->packet_->trans_rec_number);
                        	
                        	if(receiving_antenna->packet_->possible_retransmissions != wireless_network_->LR)
                        	{
                                statistics->add_retransmissions_corrected_finished(receiving_antenna->packet_->trans_rec_number, (wireless_network_->LR - receiving_antenna->packet_->possible_retransmissions));   
                        	}
                        }
                    }
                	

					// check that retransmission is possible
                    if ((receiving_antenna->packet_->possible_retransmissions <= 0) && (ter || receiving_antenna->was_collision_ == true)) //IS IMPOSSIBLE
                    {
                        int transmitter_number = receiving_antenna->packet_->trans_rec_number;

                        string text = "Retransmission of Packet is impossible (retransmission range completed). Start to transmit next packet: ";
                        logger.Info(text.append(to_string((wireless_network_->transmitting_antennae.at(transmitter_number)->packets.front()->trans_rec_number))).append(". Simulation time: ").append(to_string(static_cast<double>(this->actual_time_)/2)).append(" [ms]."), text_mode_, simu);

                        wireless_network_->delete_first_packet_from_transmitter(transmitter_number);

                        if (!(wireless_network_->transmitting_antennae.at(transmitter_number)->packets.empty()))
                        {
                            ScanConnection* sc = new ScanConnection(wireless_network_->get_front_packet_from_transmitter(transmitter_number), this->actual_time_ + wireless_network_->CTIZ, //CTIZ symbolize waiting for ACK 
                                0, transmitter_number, wireless_network_, this);
                            this->add_and_sort_events(sc);
                        }

                    	//data to statistics -> add_lose_packets(...)
                        if (phase_time <= actual_time_) { statistics->add_lose_packets(transmitter_number); }
                    }
					if ((receiving_antenna->packet_->possible_retransmissions > 0) && (ter || receiving_antenna->was_collision_ == true)) //if TER is 1 (error of transmission), or was_collision == true  AND   possible_retransmissions > 0 (IS POSSIBLE)
					{
						int transmitter_number = receiving_antenna->packet_->trans_rec_number;
						receiving_antenna->packet_->possible_retransmissions--; //decrease number of possible retransmissions

						string text = "Packet from transmitter ";
						logger.Info(text.append(to_string(transmitter_number)).append(" will be retransmitted. ").append(to_string(receiving_antenna->packet_->possible_retransmissions))
							.append(" possible retransmissions left. Simulation time: ").append(to_string(static_cast<double>(this->actual_time_)/2)).append(" [ms]."),text_mode_, simu);

						int actual_retransmission = wireless_network_->LR - (receiving_antenna->packet_->possible_retransmissions);

						ScanConnection* sc = new ScanConnection((wireless_network_->transmitting_antennae.at(transmitter_number)->packets.front()),
							this->actual_time_ + (((wireless_network_->transmitting_antennae.at(transmitter_number)->generator_CTP->Rand(1,10))*2 /*multiple *2 because, 1 loop = 0.5 ms*/) *
                                 (wireless_network_->transmitting_antennae.at(transmitter_number)->generator_R->Rand(0, static_cast<int>(pow((2), static_cast<double>(actual_retransmission)) - 1)))) + wireless_network_->CTIZ/* CTIZ time*/,
							0, transmitter_number, wireless_network_, this);

						this->add_and_sort_events(sc);
					}
					

					//condition decide about release wireless_connection if the packet with the longest execute_time appear in transmitter (work only if in connectivity was_collision_ == true) 
					if (receiving_antenna->was_collision_ == true && receiving_antenna->packet_->packed_id == pack_longest->packed_id)
					{
						wireless_network_->wireless_connection->set_acces_possible(true);
                        pack_longest = new Packet(-1, -1, -1, -1, -1);
					}
					if (receiving_antenna->was_collision_ == false && ter)
					{
                       wireless_network_->wireless_connection->set_acces_possible(true);
					}

                    no_event = false;
                    
                    receiving_antenna->packet_ = nullptr; //po sprawdzeniu warunków czy do receivera coœ trafi³o, nale¿y wyczyœciæ "bufor" danego receivera //after checking a conditions that receiver got some think, program must to clear "bufor" this receiver
                }
            }
        }

    	if(mode_==0) //if step mode is ON, the loop of program stop on this condition
    	{
            system("pause");
    	}
    }


    //saving kernel_uniform to file 
    string directory_uniform = "kernel_uniform_SIMULATION_";
    directory_uniform.append(to_string(number_of_simulation_)).append(".txt");

	if(ifstream(directory_uniform))
	{
        ofstream foutput2;
        ifstream finput2;
        finput2.open(directory_uniform);
        foutput2.open(directory_uniform, ios::app);

        if (finput2.is_open())
        {
            //cout << this->values_uniform << endl;
            foutput2 << this->values_uniform;
           // cout << endl;
        }

        finput2.close();
        foutput2.close();
	}
    else
    {
        std::ofstream file(directory_uniform);

        ofstream foutput2;
        ifstream finput2;
        finput2.open(directory_uniform);
        foutput2.open(directory_uniform, ios::app);

        if (finput2.is_open())
        {
            //cout << this->values_uniform << endl;
            foutput2 << this->values_uniform;
            //cout << endl;
        }

        finput2.close();
        foutput2.close();
    }


    //saving kernel_exponential to file
    string directory_exponential = "kernel_exponential_SIMULATION_";
    directory_exponential.append(to_string(number_of_simulation_)).append(".txt");

	if(ifstream(directory_exponential))
	{
        ofstream foutput3;
        ifstream finput3;
        finput3.open(directory_exponential);
        foutput3.open(directory_exponential, ios::app);

        if (finput3.is_open())
        {
            //cout << this->values_exponental << endl;
            foutput3 << this->values_exponental;
        }

        finput3.close();
        foutput3.close();
	}
    else
    {
        std::ofstream file(directory_exponential);

        ofstream foutput3;
        ifstream finput3;
        finput3.open(directory_exponential);
        foutput3.open(directory_exponential, ios::app);

        if (finput3.is_open())
        {
            //cout << this->values_exponental << endl;
            foutput3 << this->values_exponental;
        }

        finput3.close();
        foutput3.close();
    }
	
    
    statistics->Report();
}


