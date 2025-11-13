#include <libiw4x/component/party.hxx>

#include <iostream>
#include <sstream>
#include <cstring>

#include <libiw4x/symbol.hxx>

using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  void party::
  connect_to_party (netadr_t& a)
  {
    // Emit a party connection request to the remote endpoint.
    //
    const string r ("connect_to_party");
    NET_OutOfBandPrint (0, &a, r.c_str ());
  }

  party::
  party (oob& o)
  {
    // Server-side: handle connect_to_party from a client.
    //
    o.register_handler ("connect_to_party",
      [this] (const netadr_t& addr, const vector<string>&)
    {
      dvar_t* mapname  (Dvar_FindVar ("mapname"));
      dvar_t* gametype (Dvar_FindVar ("g_gametype"));

      ostringstream os;
      os << "connect_from_party" << " \"" << mapname->current.string  << "\""
                                 << " \"" << gametype->current.string << "\"";

      const string r (os.str ());
      NET_OutOfBandPrint (0, const_cast<netadr_t*> (&addr), r.c_str ());
    });

    // Client-side: handle connect_from_party from a server.
    //
    o.register_handler ("connect_from_party",
      [this] (const netadr_t& addr, const vector<string>& args)
    {
      const string& mapname  (args[1]);
      const string& gametype (args[2]);

      // Apply received session parameters.
      //
      Dvar_SetFromStringByName ("ui_mapname",  mapname.c_str ());
      Dvar_SetFromStringByName ("ui_gametype", gametype.c_str ());

      // Establish the party connection.
      //
      XSESSION_INFO xsi = {};
      CL_ConnectFromParty (0,
                           &xsi,
                           addr,
                           0,
                           0,
                           mapname.c_str (),
                           gametype.c_str ());
    });
  }
}
