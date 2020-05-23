#ifndef POSTGRES
#define POSTGRES

/**
 * 
 *  start ~/scripts/docker/postgres.sh 
 *  
 *  FEHLERHANDLING
 *  - docker volume rm -f postgres-vol
 * 
 */

#include "Connections.h"
#include <pqxx/pqxx>

class Postgres
{
private:
    char *connectionString;
    
public:
    Postgres(char *connectionString)
    {
        this->connectionString = connectionString;
        try {
            pqxx::connection conn(this->connectionString);
            if (conn.is_open()) {
                std::cout << "Opened database successfully: " << conn.dbname() << std::endl;

                pqxx::work w(conn);
                w.exec("truncate table game;");
                w.commit();
            }                
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            assert(false);
        }
        return;
    }

    void writeTree(Tree &tree)
    {
        try {
            pqxx::connection conn(this->connectionString);
            if (conn.is_open()) {
                pqxx::work w(conn);
                w.exec("truncate table tree;");
                w.commit();
                std::list<Move>::iterator it = tree.it_getFirst();
                int insertCount = 0;
                int nodeCount = tree.getNodeCountInTree();
                int dezel = tree.getNodeCountInTree() / 10;
                do {
                    Move move = *it;
                    char sql[1023];
                    strcpy(sql, "insert into tree VALUES (");
                    strcat(sql, std::to_string(move.getMoveId()).c_str());
                    strcat(sql, ", '");
                    strcat(sql, helper::toNot(move.getFrom()));
                    strcat(sql, "', '");
                    strcat(sql, helper::toNot(move.getTo()));
                    strcat(sql, "', ");
                    strcat(sql, std::to_string(move.getLevel()).c_str());
                    strcat(sql, ", ");
                    strcat(sql, std::to_string(move.isHit()).c_str());
                    strcat(sql, ", ");
                    strcat(sql, std::to_string(move.getEval()).c_str());
                    strcat(sql, ", ");
                    strcat(sql, std::to_string(move.isWhite()).c_str());
                    strcat(sql, ", ");
                    strcat(sql, std::to_string(move.getFirstMoveId()).c_str());
                    strcat(sql, ", ");
                    strcat(sql, std::to_string(move.getPreMoveId()).c_str());
                    strcat(sql, ", ");
                    strcat(sql, std::to_string(move.getPostMoveId()).c_str());
                    strcat(sql, ", ");

                    // build Moverow if dirst level
                    std::vector<Move> moveRow;
                    strcat(sql, "'");
                    if (move.getLevel() == 1) {
                        moveRow = tree.getMoveRow(move);
                        strcat(sql, helper::getMoveRowString(moveRow));
                    }
                    strcat(sql, "', ");
                    strcat(sql, std::to_string(move.gethash32()).c_str());
                    strcat(sql, ", '");
                    strcat(sql, (move.getDebugMsg()).c_str());
                    strcat(sql, "', ");
                    strcat(sql, "'");
                    strcat(sql, std::to_string(move.getEvalModus()).c_str());
                    /*
                    for (int i = 0; i < move.getEvalModusList().size(); i++) {
                        strcat(sql, " ");
                        strcat(sql, std::to_string(move.getEvalModusList()[i]).c_str());
                    }
                    */
                    strcat(sql, "', ");
                    strcat(sql, std::to_string(move.getTreeModus()).c_str());
                    strcat(sql, ");");
                    pqxx::work w(conn);
                    w.exec(sql);
                    w.commit();
                    if (it == tree.it_getLast()) break;
                    it++;
                    insertCount++;
                    if (insertCount % dezel == 0) {
                        printf (".");
                    }
                } while (true);
                std::cout << std::endl;
            }
            else {
                std::cout << "Can't open database" << std::endl;
                assert(false);
            }
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            assert(false);
        }
        return;
    }

    void writeGameStats(CalcMoveResult calcResult)
    {
        try {
            pqxx::connection conn(this->connectionString);
            if (conn.is_open()) {
                pqxx::work w(conn);
                char sql[127];
                strcpy(sql, "insert into game VALUES (");
                strcat(sql, std::to_string(calcResult.getMoveNr()).c_str());
                strcat(sql, ", '");
                strcat(sql, helper::getNotStr(calcResult.getBestMove()));
                strcat(sql, "');");
                w.exec(sql);
                w.commit();
            }                
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            assert(false);
        }
        return;




    }
};

#endif