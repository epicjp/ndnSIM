/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnx-path-weight-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"
#include "ns3/ccnx-app.h"
#include "ns3/ccnx-face.h"
#include "ns3/boolean.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace std;

namespace ns3 {
    
CcnxPathWeightTracer::CcnxPathWeightTracer (std::ostream &os, Ptr<Node> node, std::string appId)
  : m_os (os)
  , m_nodePtr (node)
  , m_appId (appId)
{
  m_node = boost::lexical_cast<string> (m_nodePtr->GetId ());

  Connect ();

  string name = Names::FindName (node);
  if (!name.empty ())
    {
      m_node = name;
    }
}

void
CcnxPathWeightTracer::Connect ()
{
  Config::Set ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/ForwardingStrategy/MetricTagging",
               BooleanValue (true));

  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/FaceList/*/$ns3::CcnxLocalFace/PathWeightsTrace",
                   MakeCallback (&CcnxPathWeightTracer::InLocalFace, this));
}


void
CcnxPathWeightTracer::PrintHeader (std::ostream &os) const
{
  os << "Node\t"
     << "AppId\t"
     << "PathWeight\t"
     << "PathWeights\n";
}

void
CcnxPathWeightTracer::InLocalFace (std::string context,
                                   uint32_t weight, Ptr<Node> src, Ptr<Node> dst)
{
  std::cout << "Path weights from " << Names::FindName (src) << " to "<< Names::FindName (dst) <<" : " << weight << "\n";
}

} // namespace ns3