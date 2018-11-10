#include <ut/assert.h>

#include <stdio.h>

using namespace std;

namespace ut
{
   LocationInfo::LocationInfo(const string &filename_, int line_)
      : filename(filename_), line(line_)
   {  }


   FailedAssertion::FailedAssertion(const string &message, const LocationInfo &location_)
      : logic_error(ComposeMessage(message, location_)), location(location_)
   {  }

   FailedAssertion::~FailedAssertion() throw()
   {  }

   string FailedAssertion::ComposeMessage(const string &message, const LocationInfo &location_)
   {
      char buffer[20] = { 0 };

      sprintf(buffer, "#%d: ", location_.line);
      return buffer + message;
   }
}
