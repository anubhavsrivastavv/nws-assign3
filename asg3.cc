/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 *   Copyright (c) 2017 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2 as
 *   published by the Free Software Foundation;
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/


#include "ns3/core-module.h"
#include "ns3/config-store.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/nr-module.h"
#include "ns3/config-store-module.h"
#include "ns3/antenna-module.h"
#include "ns3/nr-helper.h"
#include "ns3/log.h"
/**
 * \file cttc-3gpp-channel-nums.cc
 * \ingroup examples
 * \brief Simple topology numerologies example.
 *
 * This example allows users to configure the numerology and test the end-to-end
 * performance for different numerologies. In the following figure we illustrate
 * the simulation setup.
 *
 * For example, UDP packet generation rate can be configured by setting
 * "--lambda=1000". The numerology can be toggled by the argument,
 * e.g. "--numerology=1". Additionally, in this example two arguments
 * are added "bandwidth" and "frequency", both in Hz units. The modulation
 * scheme of this example is in test mode, and it is fixed to 28.
 *
 * By default, the program uses the 3GPP channel model, without shadowing and with
 * line of sight ('l') option. The program runs for 0.4 seconds and one single
 * packet is to be transmitted. The packet size can be configured by using the
 * following parameter: "--packetSize=1000".
 *
 * This simulation prints the output to the terminal and also to the file which
 * is named by default "cttc-3gpp-channel-nums-fdm-output" and which is by
 * default placed in the root directory of the project.
 *
 * To run the simulation with the default configuration one shall run the
 * following in the command line:
 *
 * ./waf --run cttc-3gpp-channel-nums
 *
 */

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("3gppChannelNumerologiesExample");
void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon);
std:: map<int ,std::vector<double>> flowwise;
std::ofstream outFile1;

int
main (int argc, char *argv[])
{
  // enable logging or not
//  bool logging = true;
  bool logging = false;
  if (logging)
    {
      LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
      LogComponentEnable ("LtePdcp", LOG_LEVEL_INFO);
    }

  // set simulation time and mobility
  double simTime = 5; // seconds
  double udpAppStartTime = 0.4; //seconds

  //other simulation parameters default values
  uint16_t numerology = 1;

  uint16_t gNbNum = 1;
  uint16_t ueNumPergNb = 6;

  double centralFrequency = 6e9;
  double bandwidth = 50e6;
  std::string pattern = "F|F|F|F|F|F|F|F|F|F|"; 
  std::string schedulerType = "TdmaRR";
  std::string scheduler = "";
  std::string ueMobility = "static";
  uint32_t speed = 10;
  uint32_t rngRun = 2;

  double txPower = 23;
  double lambda = 2500; //pkt generation rate or pkts/sec
  uint32_t udpPacketSize = 1500;
  bool fullBufferFlag = true;
  bool instTput = false;
  bool remPlot = false;
  bool snrLog = false;
//  uint8_t fixedMcs = 28;
//  bool useFixedMcs = false;
//  bool singleUeTopology = false;
  // Where we will store the output files.
  std::string simOutFile = "test";
//  std::string simTag = "test";
  std::string outputDir = "./outputFiles";
 

  CommandLine cmd;
  cmd.AddValue ("schedulerType", "The scheduler type to be used. Enter as OfdmaRR, TdmaRR, OfdmaPF, TdmaPF, OfdmaMR, TdmaMR", schedulerType);
  cmd.AddValue ("speed", "The speed with which the UE is moving", speed);
  cmd.AddValue ("rngRun", "The seed to be used", rngRun);
  cmd.AddValue ("numerology", "The numerology to be used.", numerology);
  cmd.AddValue ("fullBufferFlag", "Whether to set the full buffer traffic", fullBufferFlag);
  cmd.AddValue ("ueMobility", "Whether UE is static or mobile", ueMobility);
  cmd.AddValue ("ueNumPergNb", "Number of UE per gnb", ueNumPergNb);
  cmd.AddValue ("simTime", "Simulation Time", simTime);
  cmd.AddValue ("outputDir", "directory where to store simulation results", outputDir);
  cmd.AddValue ("simOutFile", "file where to store simulation results", simOutFile);
  cmd.AddValue ("instTput", "Specifies if we need to retieve instantaneous throughput of UE", instTput);
  cmd.AddValue ("snrLog", "Specifies if snr logging is enabled", snrLog);
  cmd.AddValue ("remPlot", "Specifies if snr logging is enabled", remPlot);

  cmd.Parse (argc, argv);
  
  scheduler = "ns3::NrMacScheduler" + schedulerType;
  printf("final scheduler is %s",scheduler.c_str());

  if(numerology == 0 || numerology == 1 || numerology == 2)
  {
      centralFrequency = 6e9;
  }
  else if(numerology == 3)
  {
      centralFrequency = 28e9;
  }

  if (fullBufferFlag)
  {
      lambda = 2500;
  }
  else
  {
      lambda = 1000;
  }

  if(logging)
  {
     // LogComponentEnable ("NrRadioEnvironmentMapHelper", LOG_LEVEL_INFO);
  }
 printf("sim time is %f", simTime);
  NS_ASSERT (ueNumPergNb > 0);

  // setup the nr simulation
  Ptr<NrHelper> nrHelper = CreateObject<NrHelper> ();

  /*
   * Spectrum division. We create one operation band with one component carrier
   * (CC) which occupies the whole operation band bandwidth. The CC contains a
   * single Bandwidth Part (BWP). This BWP occupies the whole CC band.
   * Both operational bands will use the StreetCanyon channel modeling.
   */
  CcBwpCreator ccBwpCreator;
  const uint8_t numCcPerBand = 1;  // in this example, both bands have a single CC
  BandwidthPartInfo::Scenario scenario = BandwidthPartInfo::UMa_LoS;



  // Create the configuration for the CcBwpHelper. SimpleOperationBandConf creates
  // a single BWP per CC
  CcBwpCreator::SimpleOperationBandConf bandConf (centralFrequency,
                                                  bandwidth,
                                                  numCcPerBand,
                                                  scenario);

  // By using the configuration created, it is time to make the operation bands
  OperationBandInfo band = ccBwpCreator.CreateOperationBandContiguousCc (bandConf);

  /*
   * Initialize channel and pathloss, plus other things inside band1. If needed,
   * the band configuration can be done manually, but we leave it for more
   * sophisticated examples. For the moment, this method will take care
   * of all the spectrum initialization needs.
   */
  nrHelper->InitializeOperationBand (&band);

  BandwidthPartInfoPtrVector allBwps = CcBwpCreator::GetAllBwps ({band});

  /*
   * Continue setting the parameters which are common to all the nodes, like the
   * gNB transmit power or numerology.
   */
  nrHelper->SetGnbPhyAttribute ("TxPower", DoubleValue (txPower));
  nrHelper->SetGnbPhyAttribute ("Numerology", UintegerValue (numerology));
  nrHelper->SetGnbPhyAttribute ("Pattern", StringValue (pattern));

  // Scheduler

  nrHelper->SetSchedulerTypeId (TypeId::LookupByName (scheduler));
//  nrHelper->SetSchedulerAttribute ("FixedMcsDl", BooleanValue (useFixedMcs));
//  nrHelper->SetSchedulerAttribute ("FixedMcsUl", BooleanValue (useFixedMcs));
/*
  if (useFixedMcs == true)
    {
      nrHelper->SetSchedulerAttribute ("StartingMcsDl", UintegerValue (fixedMcs));
      nrHelper->SetSchedulerAttribute ("StartingMcsUl", UintegerValue (fixedMcs));
    }
*/
  Config::SetDefault ("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue (999999999));

  // Antennas for all the UEs
  nrHelper->SetUeAntennaAttribute ("NumRows", UintegerValue (2));
  nrHelper->SetUeAntennaAttribute ("NumColumns", UintegerValue (4));
  nrHelper->SetUeAntennaAttribute ("AntennaElement", PointerValue (CreateObject<IsotropicAntennaModel> ()));

  // Antennas for all the gNbs
  nrHelper->SetGnbAntennaAttribute ("NumRows", UintegerValue (4));
  nrHelper->SetGnbAntennaAttribute ("NumColumns", UintegerValue (8));
  nrHelper->SetGnbAntennaAttribute ("AntennaElement", PointerValue (CreateObject<ThreeGppAntennaModel> ()));

  // Beamforming method
  Ptr<IdealBeamformingHelper> idealBeamformingHelper = CreateObject<IdealBeamformingHelper>();
  idealBeamformingHelper->SetAttribute ("BeamformingMethod", TypeIdValue (DirectPathBeamforming::GetTypeId ()));
  nrHelper->SetBeamformingHelper (idealBeamformingHelper);

  Config::SetDefault ("ns3::ThreeGppChannelModel::UpdatePeriod",TimeValue (MilliSeconds (0)));
//  nrHelper->SetChannelConditionModelAttribute ("UpdatePeriod", TimeValue (MilliSeconds (0)));
  nrHelper->SetPathlossAttribute ("ShadowingEnabled", BooleanValue (false));

  // Error Model: UE and GNB with same spectrum error model.
  nrHelper->SetUlErrorModel ("ns3::NrEesmIrT1");
  nrHelper->SetDlErrorModel ("ns3::NrEesmIrT1");

  // Both DL and UL AMC will have the same model behind.

  nrHelper->SetGnbDlAmcAttribute ("AmcModel", EnumValue (NrAmc::ShannonModel)); // NrAmc::ShannonModel or NrAmc::ErrorModel
  nrHelper->SetGnbUlAmcAttribute ("AmcModel", EnumValue (NrAmc::ShannonModel)); // NrAmc::ShannonModel or NrAmc::ErrorModel


  // Create EPC helper
  Ptr<NrPointToPointEpcHelper> epcHelper = CreateObject<NrPointToPointEpcHelper> ();
  nrHelper->SetEpcHelper (epcHelper);
  // Core latency
  epcHelper->SetAttribute ("S1uLinkDelay", TimeValue (MilliSeconds (2)));

  // gNb routing between Bearer and bandwidh part
  uint32_t bwpIdForBearer = 0;
  nrHelper->SetGnbBwpManagerAlgorithmAttribute ("GBR_CONV_VOICE", UintegerValue (bwpIdForBearer));

  // Initialize nrHelper
  nrHelper->Initialize ();


  /*
   *  Create the gNB and UE nodes according to the network topology
   */
  NodeContainer gNbNodes;
  NodeContainer ueNodes;
  gNbNodes.Create (gNbNum);
  ueNodes.Create (ueNumPergNb * gNbNum);
  const double gNbHeight = 10;
  const double ueHeight = 1.5;

  MobilityHelper gnbMobility;
  
  gnbMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  Ptr<ListPositionAllocator> gnbPositionAlloc = CreateObject<ListPositionAllocator> ();
  gnbPositionAlloc->Add (Vector (0.0, 0, gNbHeight));
  gnbMobility.SetPositionAllocator (gnbPositionAlloc);
  gnbMobility.Install (gNbNodes);

  MobilityHelper udMobility;
  Ptr<ListPositionAllocator> uePositionAlloc = CreateObject<ListPositionAllocator> ();

  if (ueMobility == "static")
  {
      udMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  }
  else if(ueMobility == "mobile")
  {
      if (speed == 10)
      {
          udMobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel", "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=10]"),
                  "Bounds", RectangleValue (Rectangle (-3000, 3000, -3000, 3000)));
      }
      else if (speed == 50)
      {
          udMobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel", "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=50]"),
                  "Bounds", RectangleValue (Rectangle (-3000, 3000, -3000, 3000)));
      }
 }

  if (ueNumPergNb == 6)
  {
      uePositionAlloc->Add (Vector (10, 0, ueHeight));
      uePositionAlloc->Add (Vector (1000, 0, ueHeight));
      uePositionAlloc->Add (Vector (3000, 0, ueHeight));
      uePositionAlloc->Add (Vector (-10, 0, ueHeight));
      uePositionAlloc->Add (Vector (-1000, 0, ueHeight));
      uePositionAlloc->Add (Vector (-3000,0, ueHeight));
  }
  else
  {
      Ptr<UniformRandomVariable> v = CreateObject<UniformRandomVariable> ();

      double xValue = 0.0;
      double min = 10.0;
      double max = 3000.0;


   //   std::cout<<"Random numbers using GetValue are \n";
//  std::cout<<"Random numbers using GetInteger are \n";
      for (uint16_t j = 1; j <= ueNumPergNb; j++)
      { 
          std::cout<<"\t"<< v->GetValue (10.0, 3000.0);
          xValue = v->GetValue(min, max);
          uePositionAlloc->Add (Vector (xValue, 0, ueHeight));

  //    std::cout<<"\t"<<v->GetInteger (min, max);
      }
/*
      double xValue = 0.0;
      for (uint16_t j = 1; j <= ueNumPergNb; ++j)
      {
          xValue = 10 + 2*j;
          uePositionAlloc->Add (Vector (xValue, 0, ueHeight));
      }
  */    
  }

  udMobility.SetPositionAllocator (uePositionAlloc);
  udMobility.Install (ueNodes);

   // Install nr net devices
  NetDeviceContainer gNbNetDev = nrHelper->InstallGnbDevice (gNbNodes,
                                                             allBwps);

  NetDeviceContainer ueNetDev = nrHelper->InstallUeDevice (ueNodes,
                                                           allBwps);

  int64_t randomStream = 2;
  randomStream += nrHelper->AssignStreams (gNbNetDev, rngRun);
  randomStream += nrHelper->AssignStreams (ueNetDev, rngRun);


  // When all the configuration is done, explicitly call UpdateConfig ()

  for (auto it = gNbNetDev.Begin (); it != gNbNetDev.End (); ++it)
    {
      DynamicCast<NrGnbNetDevice> (*it)->UpdateConfig ();
    }

  for (auto it = ueNetDev.Begin (); it != ueNetDev.End (); ++it)
    {
      DynamicCast<NrUeNetDevice> (*it)->UpdateConfig ();
    }


  // create the internet and install the IP stack on the UEs
  // get SGW/PGW and create a single RemoteHost
  Ptr<Node> pgw = epcHelper->GetPgwNode ();
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // connect a remoteHost to pgw. Setup routing too
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("10Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (2500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.005)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);
  internet.Install (ueNodes);


  Ipv4InterfaceContainer ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueNetDev));

  // Set the default gateway for the UEs
  for (uint32_t j = 0; j < ueNodes.GetN (); ++j)
    {
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNodes.Get (j)->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }

  // attach UEs to the closest eNB
  nrHelper->AttachToClosestEnb (ueNetDev, gNbNetDev);

  // assign IP address to UEs, and install UDP downlink applications
  uint16_t dlPort = 1234;

  ApplicationContainer serverApps;

  // The sink will always listen to the specified ports
  UdpServerHelper dlPacketSinkHelper (dlPort);
  serverApps.Add (dlPacketSinkHelper.Install (ueNodes.Get (0)));

  UdpClientHelper dlClient;
  dlClient.SetAttribute ("RemotePort", UintegerValue (dlPort));
  dlClient.SetAttribute ("PacketSize", UintegerValue (udpPacketSize));
  dlClient.SetAttribute ("MaxPackets", UintegerValue (0xFFFFFFFF));
  dlClient.SetAttribute ("Interval", TimeValue (Seconds (1.0 / lambda)));

  // The bearer that will carry low latency traffic
  EpsBearer bearer (EpsBearer::GBR_CONV_VOICE);

  Ptr<EpcTft> tft = Create<EpcTft> ();
  EpcTft::PacketFilter dlpf;
  dlpf.localPortStart = dlPort;
  dlpf.localPortEnd = dlPort;
  tft->Add (dlpf);

  /*
   * Let's install the applications!
   */
  ApplicationContainer clientApps;

  for (uint32_t i = 0; i < ueNodes.GetN (); ++i)
  {
      Ptr<Node> ue = ueNodes.Get (i);
      Ptr<NetDevice> ueDevice = ueNetDev.Get (i);
      Address ueAddress = ueIpIface.GetAddress (i);

      // The client, who is transmitting, is installed in the remote host,
      // with destination address set to the address of the UE
      dlClient.SetAttribute ("RemoteAddress", AddressValue (ueAddress));
      clientApps.Add (dlClient.Install (remoteHost));

      // Activate a dedicated bearer for the traffic type
      nrHelper->ActivateDedicatedEpsBearer (ueDevice, bearer, tft);
    }

  // start server and client apps
  serverApps.Start (Seconds (udpAppStartTime));
  clientApps.Start (Seconds (udpAppStartTime));
  serverApps.Stop (Seconds (simTime));
  clientApps.Stop (Seconds (simTime));


  // enable the traces provided by the nr module
  //nrHelper->EnableTraces();


  FlowMonitorHelper flowmonHelper;
  NodeContainer endpointNodes;
  endpointNodes.Add (remoteHost);
  endpointNodes.Add (ueNodes);

  Ptr<ns3::FlowMonitor> monitor = flowmonHelper.Install (endpointNodes);
  monitor->SetAttribute ("DelayBinWidth", DoubleValue (0.001));
  monitor->SetAttribute ("JitterBinWidth", DoubleValue (0.001));
  monitor->SetAttribute ("PacketSizeBinWidth", DoubleValue (20));

  Ptr<NrRadioEnvironmentMapHelper> remHelper = CreateObject<NrRadioEnvironmentMapHelper> ();
  if(remPlot)
  {
      // configure REM parameters
      if (snrLog)
      {
          LogComponentEnable ("NrRadioEnvironmentMapHelper", LOG_LEVEL_INFO);
      }

      //Rem parameters
      double xMin = -3000.0;
      double xMax = 3000.0;
      uint16_t xRes = 100;
      double yMin = -3000.0;
      double yMax = 3000.0;
      uint16_t yRes = 100;
      std::string simTag = "";

      remHelper->SetMinX (xMin);
      remHelper->SetMaxX (xMax);
      remHelper->SetResX (xRes);
      remHelper->SetMinY (yMin);
      remHelper->SetMaxY (yMax);
      remHelper->SetResY (yRes);
      remHelper->SetSimTag (simTag);
      remHelper->SetRemMode (NrRadioEnvironmentMapHelper::BEAM_SHAPE);

      //configure beam that will be shown in REM map
      //DynamicCast<NrGnbNetDevice> (gNbNetDev.Get (0))->GetPhy (0)->GetBeamManager ()->SetSector (sector, theta);
      for(int i=0 ; i<ueNumPergNb ; i++){
          //DynamicCast<NrUeNetDevice> (ueNetDev.Get (i))->GetPhy (0)->GetBeamManager ()->ChangeToQuasiOmniBeamformingVector ();
          gNbNetDev.Get(0)->GetObject<NrGnbNetDevice>()->GetPhy(0)->GetBeamManager()->ChangeBeamformingVector(ueNetDev.Get(i));
          remHelper->CreateRem (gNbNetDev,ueNetDev.Get(i), 0);
      }

  }

  if(instTput)
  {
      std::string filename1 = outputDir + "/" + "Q10-throughPuts" + schedulerType;
      outFile1.open (filename1.c_str (), std::ofstream::out | std::ofstream::app);
      if (!outFile1.is_open ())
      {
          NS_LOG_ERROR ("Can't open file " << filename1);
          return 1;
      }
      outFile1.setf (std::ios_base::fixed);
      Simulator::Schedule(Seconds(0.001),&ThroughputMonitor,&flowmonHelper, monitor);
  }
  Simulator::Stop (Seconds (simTime));
  Simulator::Run ();



  // Print per-flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmonHelper.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();

  double averageFlowThroughput = 0.0;
  double averageFlowDelay = 0.0;

//  AnimationInterface anim("hand.xml"); 

  std::ofstream outFile;
  std::string filename = outputDir + "/" + simOutFile;
  outFile.open (filename.c_str (), std::ofstream::out | std::ofstream::app);
  if (!outFile.is_open ())
    {
      NS_LOG_ERROR ("Can't open file " << filename);
      return 1;
    }
  outFile.setf (std::ios_base::fixed);


/*
	Creating output file in the json format:
*/
  outFile << "{\n";
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      std::stringstream protoStream;
      protoStream << (uint16_t) t.protocol;
      if (t.protocol == 6)
        {
          protoStream.str ("TCP");
        }
      if (t.protocol == 17)
        {
          protoStream.str ("UDP");
        }		
		
      outFile << "\"Flow" << i->first << "\" : "; 
	  outFile << "{\n";
	  outFile << "	\"src\" : \"" << t.sourceAddress << ":" << t.sourcePort << "\",\n";
	  outFile << "	\"dest\" : \"" << t.destinationAddress << ":" << t.destinationPort << "\",\n";
	  outFile << "	\"protocol\" : \"" << protoStream.str () << "\",\n";
      outFile << "	\"Tx Packets\" : \"" << i->second.txPackets << "\",\n";
      outFile << "	\"Tx Bytes\" : \"" << i->second.txBytes << "\",\n";
      outFile << "	\"TxOffered\" : \"" << i->second.txBytes * 8.0 / (simTime - udpAppStartTime) / 1000 / 1000  << " Mbps\",\n";
      outFile << "	\"Rx Bytes\" : \"" << i->second.rxBytes << "\",\n";
      if (i->second.rxPackets > 0)
        {
          // Measure the duration of the flow from receiver's perspective
          double rxDuration = i->second.timeLastRxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds ();

          averageFlowThroughput += i->second.rxBytes * 8.0 / rxDuration / 1000 / 1000;
          averageFlowDelay += 1000 * i->second.delaySum.GetSeconds () / i->second.rxPackets;

          outFile << "	\"Throughput\" : \"" << i->second.rxBytes * 8.0 / rxDuration / 1000 / 1000  << " Mbps\",\n";
          outFile << "	\"Mean delay\" : \"" << 1000 * i->second.delaySum.GetSeconds () / i->second.rxPackets << " ms\",\n";
          //outFile << "  Mean upt:  " << i->second.uptSum / i->second.rxPackets / 1000/1000 << " Mbps \n";
          outFile << "	\"Mean jitter\" : \"" << 1000 * i->second.jitterSum.GetSeconds () / i->second.rxPackets  << " ms\",\n";
        }
      else
        {
          outFile << "	\"Throughput\" : \"0 Mbps\",\n";
          outFile << "	\"Mean delay\" : \"0 ms\",\n";
          outFile << "	\"Mean upt\" : \"0 Mbps\",\n";
          outFile << "	\"Mean jitter\" : \"0 ms\",\n";
        }
      outFile << "	\"Rx Packets\" : \"" << i->second.rxPackets << "\",\n";
//      double lossRate = 100 * (i->second.txPackets - i->second.rxPackets)/i->second.txPackets;
//      outFile << "  Loss Rate: " << lossRate << "\n";
      outFile << "	\"Loss Rate\" : \"" << 100.00 * (i->second.txPackets - i->second.rxPackets)/i->second.txPackets << "\"\n";
	  outFile << "},\n";
    }

  outFile << "\n\"Mean flow throughput\" : \"" << averageFlowThroughput / stats.size () << "\",\n";
  outFile << "\"Mean flow delay\" : \"" << averageFlowDelay / stats.size () << "\"\n";
  outFile << "}\n";
  outFile << "==================\n";
  outFile.close ();
//  for (uint32_t i = 0; i < ueNodes.GetN (); ++i)
//  { 

  Ptr<UdpClient> clientApp = clientApps.Get (0)->GetObject<UdpClient> ();
  Ptr<UdpServer> serverApp = serverApps.Get (0)->GetObject<UdpServer> ();
  std::cout << "\n Total UDP throughput (bps):" <<
  (serverApp->GetReceived () * udpPacketSize * 8) / (simTime - udpAppStartTime) << std::endl;
//  }
  if(instTput)
  {
      ThroughputMonitor(&flowmonHelper, monitor);
  }
  Simulator::Destroy ();

  if(instTput)
  {
      for(unsigned int i=0;i<flowwise.size();i++)
      {
          // std::cout<<flowwise[0].size();
          if(i==1)
          {
              for(unsigned int j=0;j<flowwise[i].size();j++)
                  outFile1 << flowwise[1][j]<<"\n";
              //            std::cout<<flowwise[1][j]<<"\n";
              std::cout<<"\n";
          }

      }
  }
  return 0;
}

void ThroughputMonitor (FlowMonitorHelper *fmhelper, Ptr<FlowMonitor> flowMon)
{
	std::map<FlowId, FlowMonitor::FlowStats> flowStats = flowMon->GetFlowStats();
	Ptr<Ipv4FlowClassifier> classing = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats)
	{
		//if(FlowId)
		double throughput=stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds()-stats->second.timeFirstTxPacket.GetSeconds())/1024/1024;
		flowwise[stats->first].push_back(throughput);

		/*
		Ipv4FlowClassifier::FiveTuple fiveTuple = classing->FindFlow (stats->first);
		std::cout<<"Flow ID			: " << stats->first <<" ; "<< fiveTuple.sourceAddress <<" -----> "<<fiveTuple.destinationAddress<<std::endl;
		std::cout<<"Duration		: "<<stats->second.timeLastRxPacket.GetSeconds()-stats->second.timeFirstTxPacket.GetSeconds()<<std::endl;
		std::cout<<"Last Received Packet	: "<< stats->second.timeLastRxPacket.GetSeconds()<<" Seconds"<<std::endl;
		std::cout<<"Throughput: " << throughput  << " Mbps"<<std::endl;
		std::cout<<"---------------------------------------------------------------------------"<<std::endl;
		*/
	}
//	std::cout<<"Length of flowwise[1]="<<flowwise[1].size()<<"\n";

	Simulator::Schedule(Seconds(0.001),&ThroughputMonitor, fmhelper, flowMon);

}

