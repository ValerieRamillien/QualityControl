///
/// \file   MySqlDatabase.h
/// \author Barthelemy von Haller
///

#ifndef QUALITYCONTROL_REPOSITORY_MYSQL_DATABASE_H_
#define QUALITYCONTROL_REPOSITORY_MYSQL_DATABASE_H_

#include "QualityControl/DatabaseInterface.h"
#include "TMySQLServer.h"
#include "Common/Timer.h"

class TMySQLResult;

namespace o2 {
namespace quality_control {
namespace repository {

/// \brief Implementation of the DatabaseInterface for MySQL
/// \todo consider storing directly the TObject, not the MonitorObject, and to put all its attributes as columns
/// \todo handle ROOT IO streamers
class MySqlDatabase: public DatabaseInterface
{
  public:
    /// Default constructor
    MySqlDatabase();
    /// Destructor
    ~MySqlDatabase() override;

//    void connect(std::string username, std::string password) override;
    void connect(std::string host, std::string database, std::string username, std::string password) override;
    void store(o2::quality_control::core::MonitorObject* mo) override;
    o2::quality_control::core::MonitorObject* retrieve(std::string taskName, std::string objectName) override;
    void disconnect() override;
    std::vector<std::string> getPublishedObjectNames(std::string taskName) override;
    std::vector<std::string> getListOfTasksWithPublications() override;

  private:
    /**
     * \brief Execute the query.
     * The result object must be deleted by the user.
     */
    TMySQLResult* query(std::string sql);

    /**
     * \brief Execute a query that doesn't return results;
     * Return true if successful.
     */
    bool execute(std::string sql);

    /**
     * \brief Create a new index on table 'table'.
     * The name of the index is <table>_i_<column>.
     */
    void addIndex(std::string table, std::string column);

    void prepareTaskDataContainer(std::string taskName) override;

    void storeQueue();
    void storeForTask(std::string taskName);

    TMySQLServer* mServer;

    // Queue
    // name of tasks -> vector of mo
    std::map<std::string, std::vector<o2::quality_control::core::MonitorObject*>> mObjectsQueue;
    size_t queueSize;
    AliceO2::Common::Timer lastStorage;
};

} // namespace core
} // namespace QualityControl
} // namespace o2

#endif // QUALITYCONTROL_REPOSITORY_MYSQL_DATABASE_H_
