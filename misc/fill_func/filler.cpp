#include "filler.h"
#include <stdio.h>
#include <postgresql/libpq-fe.h>

bool get_values_from_outside(const std::vector<std::string> &keys,std::vector<std::string> &values)
{ 
  PGconn *conn;
  PGresult *res;
  std::basic_string<char> *command;
  
  conn = PQconnectdb("host=localhost port=5432 user=plus1 dbname=plus1");
  if (PQstatus(conn) != CONNECTION_OK) {
    PQfinish(conn);
    return false;
  }

  std::vector<std::string>::const_iterator it_key = keys.begin();
  values.erase(values.begin(), values.end());
  while (it_key != keys.end())
  {
    *command = "SELECT * FROM gdi_device WHERE id=\'" + *it_key + "\' LIMIT 1";
    res = PQexec(conn, command->c_str());
    
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
      values.push_back("b:1;"); // serialized TRUE
    } else {
      values.push_back("N;"); // serialized NULL
    }
    
    PQclear(res);

    ++it_key;
  }

  PQfinish(conn);
  return true;
}
