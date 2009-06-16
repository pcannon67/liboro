/*
 * ©LAAS-CNRS (2008-2009)
 *
 * contributor(s) : Séverin Lemaignan <severin.lemaignan@laas.fr>
 *
 * This software is a computer program whose purpose is to interface
 * with an ontology in a robotics context.
 *
 * This software is governed by the CeCILL  license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
*/

#ifndef ORO_H_
#define ORO_H_

/** \mainpage liboro: a C++ wrapper for the OpenRobots Ontology server
 * This library is a wrapper that enables programmers to easily interface their applications with the LAAS' OpenRobots Ontology server (\p oro-server ). It allows to easily declare, update, remove concepts, to check that the knowledge base is consistent, to look for specific concepts or execute complex queries on the ontology.
 * 
 * The current version (0.4.x) relies on YARP for remote procedure calls (RPC) with \p oro-server .
 * 
 * This is a simple example that assumes the ontology server is loaded with the OpenRobots ontology \p oro (\link ../../wiki/doku.php?id=openrobotsontology more details...\endlink):
 * \code
 * #include <iostream>
 * #include <iterator>
 * #include "oro.h"
 * #include "oro_library.h"
 * #include "yarp_connector.h"
 *
 * using namespace std;
 * using namespace oro;
 * int main(void) {
 * 		vector<Concept> result;
 * 		vector<string> partial_stmts;
 *
 * 		YarpConnector connector("myRobot", "oro"); //liboro currently relies on YARP for the RPCs with the server.
 * 		Ontology *oro = Ontology::createWithConnector(connector); //actually connect the application to the ontology server. The "oro" object is here built as a singleton.
 * 
 *		//First, create some instances (ie, objects).
  * 		Agent robot1 = Agent::create("Nice Robot", Classes::Robot); //a new instance of Agent has been created. It is named "Nice Robot" and its type (or "class") is set to be a Robot (which is a subconcept of Agent).
 * 		Agent human = Agent::create("Young PhD", Classes::Human); //another agent...
 *		
 * 		Object table = Object::create(Classes::Table); //here, an object is created. No name (or "label") has been set up, but the class is refined: it's not only an object, but more precisely a table.
 * 		Object unknown_object = Object::create(); //here, an unknown object has been identified, without any more infos.
 * 		unknown_object.setColor(212); //the Object class offers some high-level setters like setColor. See the class documentation for the list.
 * 
 *		//if no setter is available for a given property, then direct assertion can be made.
 *		//the list of existing properties and classes come from the oro ontology itself (from which oro_library.h/cpp is automatically generated)
 * 		unknown_object.assert(Properties::isOnTopOf, table);
 * 
 * 		Agent myself("myself"); //"myself" is a special, unique instance representing the robot itself. This instance is built from the already existing identifier "myself". Hence the constructor of the Agent class can be directly called.
 * 
 * 		myself.sees(unknown_object);
 * 		myself.sees(human);
 * 
 *		//Then, try to find back the unknown object...
 * 		partial_stmts.push_back("?mysterious oro:isAt ?table");
 * 		partial_stmts.push_back("?table rdf:type oro:Table");
 * 		partial_stmts.push_back("oro:myself oro:sees ?mysterious");
 *
 * 		oro->find("mysterious", partial_stmts, result);
 * 
 *		//display the results on std_out
 * 		copy(result.begin(), result.end(), ostream_iterator<Concept>(cout, "\n"));
 * 
 * 		return 0;
 * }
 * \endcode
 * 
 * Same example, but at a "lower" level:
 * \code
 * #include <iostream>
 * #include <iterator>
 * #include "oro.h"
 * #include "yarp_connector.h"
 *
 * using namespace std;
 * using namespace oro;
 * int main(void) {
 * 		vector<Concept> result;
 * 		vector<Statement> assertions;
 * 		vector<string> partial_stmts;
 *
 * 		YarpConnector connector("myRobot", "oro");
 * 		Ontology *oro = Ontology::createWithConnector(connector);
 * 
 *		//First, add assertions to the ontology (namespaces are not mandatory when it's the default namespace, as configured in the server config file).
 * 		assertions.push_back(Statement::create("robot1 rdf:type Robot"));
 * 		assertions.push_back(Statement::create("robot1 rdfs:label \"Nice Robot\""));
 * 		assertions.push_back(Statement::create("human rdf:type Human"));
 * 		assertions.push_back(Statement::create("human rdfs:label \"Young PhD\""));
 * 		assertions.push_back(Statement::create("table rdf:type Table"));
 * 		assertions.push_back(Statement::create("object rdf:type Object"));
 * 		assertions.push_back(Statement::create("object hasColor color"));
 * 		assertions.push_back(Statement::create("color hue 212"));
 * 		assertions.push_back(Statement::create("object isOnTopOf table"));
 * 		assertions.push_back(Statement::create("myself sees object"));
 * 		assertions.push_back(Statement::create("myself sees human"));
 * 
 * 		oro->add(assertions);
 * 
 *		//Then, try to find back the unknown object...
 * 		partial_stmts.push_back("?mysterious oro:isAt ?table");
 * 		partial_stmts.push_back("?table rdf:type oro:Table");
 * 		partial_stmts.push_back("oro:myself oro:sees ?mysterious");
 *
 * 		oro->find("mysterious", partial_stmts, result);
 * 
 *		//display the results on std_out
 * 		copy(result.begin(), result.end(), ostream_iterator<Concept>(cout, "\n"));
 * 
 * 		return 0;
 * }
 * \endcode
 * 
 * When several statements are to be inserted (or removed) from the ontology at the same time, it's strongly advised to bufferize the IO. Performances will be greatly improved. Cf Ontology::bufferize().
 * In the previous example, use of bufferize would look like that:
 * \code
 * ...
 * oro->bufferize();
 * oro->add(assertions);
 * oro->flush();
 * ...
 * \endcode
 * 
 * \author S. Lemaignan - severin.lemaignan@laas.fr
 * \date 2009
 */

/** \file
 * This is the main header of the \p liboro library. All the core classes like Ontology, Class, Property, Concept are defined here. This header is mandatory when you want to work with \p liboro .
 */

// #define ORO_VERSION "0.3.2" //defined at compile time by cmake (cf conf/LiboroVersion.cmake to change it)

//a simple macro to display the symbolic name of enums
#define NAME_OF(x) #x

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <typeinfo>

#include <boost/logic/tribool.hpp>

#include "oro_exceptions.h"
#include "oro_connector.h"

//TODO To be replaced with a proper lib
#include "position.h"

/**
 * The main \p liboro namespace.
 */
namespace oro {

class Concept;
class Statement;

/**
 * This represent the ontology itself. This class offers tools to look for concept, etc.
 */
class Ontology {

	public:
		
		/** Constants that defines the way an event occuring in the ontology is triggered.\n
		* 
		* <ul>
		*  <li>\p ON_TRUE : the event is triggered each time the corresponding watch expression <em>becomes</em> true.</li>
		*  <li>\p ON_TRUE_ONE_SHOT : the event is triggered the first time the corresponding watch expression <em>becomes</em> true. The watcher is then deleted.</li>
		*  <li>\p ON_FALSE : the event is triggered each time the corresponding watch expression <em>becomes</em> false.</li>
		*  <li>\p ON_FALSE_ONE_SHOT : the event is triggered the first time the corresponding watch expression <em>becomes</em> false. The watcher is then deleted.</li>
		*  <li>\p ON_TOGGLE : the event is triggered each time the corresponding watch expression <em>becomes</em> true or false.</li>
		* </ul>
		* 
	 	* \see subscribe(const std::string&, EventTriggeringType, const std::string&)
		*/
		//When adding new trigger type, remember to update as well the implementation of Ontology::subscribe
		enum EventTriggeringType {ON_TRUE, ON_TRUE_ONE_SHOT, ON_FALSE, ON_FALSE_ONE_SHOT, ON_TOGGLE};
		
		/**
	 	* This static getter for the ontology must be called once to initialize the ontology server.
	 	* After, the ontology instance can be accessed by a simple Oro::getInstance();
		 */
		static Ontology* createWithConnector(IConnector& connector);

		/**
		 * Return a pointer to the current instance of the ontology server.
		 * The server must be initialized by Oro::getInstance(local_port_name, oro_distant_server);
		 *
		 * Throws UninitializedOntologyException if called before initialization.
		*/
		static Ontology* getInstance();
		
		/**
		 * Adds a new statement to the ontology.\n
		 * Interface to the \p oro-server \link https://www.laas.fr/~slemaign/doc/oro-server/laas/openrobots/ontology/backends/IOntologyBackend.html#add(java.lang.String) OpenRobotsOntology#add(String) method \endlink. Please follow the link for details.\n
		 *
		* Example:
		 * \code
		 * #include "oro.h"
		 * #include "yarp_connector.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 *
		 * 		YarpConnector connector("myDevice", "oro");
		 * 		oro = Ontology::createWithConnector(connector);
		 *
		 * 		oro->add(Statement::create("gorilla rdf:type Monkey"));
		 * 		oro->add(Statement::create("gorilla age 12^^xsd:int"));
		 * 		oro->add(Statement::create("gorilla weight 75.2"));
		 *
		 * 		return 0;
		 * }
		 * \endcode
		 */
		void add(const Statement& statement);
	
		/**
		 * Adds a set of statements to the ontology.
		 * Like \p add(String) but for sets of statements. Please note that no automatic buffering is done. If you're adding several statements, you're strongly advised to use Ontology::bufferize() and Ontology::flush() (cf example below).\n
		 *
		* Example:
		 *
		 * \code
		 * #include "oro.h"
		 * #include "yarp_connector.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 *
		 * 		vector<Statement> stmts;
		 *
		 * 		YarpConnector connector("myDevice", "oro");
		 * 		oro = Ontology::createWithConnector(connector);
		 *
		 * 		stmts.push_back(Statement::create("gorilla rdf:type Monkey"));
		 * 		stmts.push_back(Statement::create("gorilla age 12^^xsd:int"));
		 * 		stmts.push_back(Statement::create("gorilla weight 75.2"));
		 *
		 * 		oro->bufferize();
		 * 		oro->add(stmts);
		 * 		oro->flush();
		 *
		 * 		return 0;
		 * }
		 * \endcode
		 */
		void add(const std::vector<Statement>& statements);
		
		/**
		 * Removes a set of statements from the ontology. Silently ignore statements that don't exist.\n
		 * Like \p remove(String) but for sets of statements. Please note that no automatic buffering is done. If you're removing several statements, you're strongly advised to use Ontology::bufferize() and Ontology::flush() before and after the call to \p remove() .\n
		*/
		void remove(const std::vector<Statement>& statements);
		
		/**
		 * Removes one statements from the ontology. Does nothing is the statement doesn't exist.
		 */
		void remove(const Statement& statement);
	
		/**
		 * Checks the ontology consistency.
		 * 
		 * \return \p true or \p false
		 */
		bool checkConsistency();		
		
		/**
		 * Tries to identify a resource given a set of partially defined statements plus restrictions about this resource.
		 * Wrapper for the server's \p laas.openrobots.ontology.OpenRobotsOntology#find(String,Vector,Vector) method. Please follow the link for details.\n
		 *
		 * Code snippet:
		 *
		 * \code
		 * #include "oro.h"
		 * #include "yarp_connector.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 * 		vector<Concept> result;
		 * 		vector<string> partial_stmts;
		 * 		vector<string> filters;
		 *
		 * 		YarpConnector connector("myDevice", "oro");
		 * 		oro = Ontology::createWithConnector(connector);
		 *
		 * 		partial_stmts.push_back("?mysterious rdf:type oro:Monkey");
		 * 		partial_stmts.push_back("?mysterious oro:weight ?value");
		 *
		 * 		filters.push_back("?value >= 50");
		 *
		 * 		oro->find("mysterious", partial_stmts, filters, result);
		 * 
		 *		//display the results on std_out
		 * 		copy(result.begin(), result.end(), ostream_iterator<Concept>(cout, "\n"));
		 * 
		 * 		return 0;
		 * }
		 * \endcode
 		*/
		void find(const std::string& resource, const std::vector<std::string>& partial_statements, const std::vector<std::string>& restrictions, std::vector<Concept>& result);

		/**
		 * Tries to identify a resource given a set of partially defined statements about this resource.
		 * Wrapper for to {@link laas.openrobots.ontology.OpenRobotsOntology#find(String, Vector)}. Please follow the link for details.\n
		 *
 		 * Working code snippet:
		 *
		 * \code
		 * #include "oro.h"
		 * #include "yarp_connector.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 * 		vector<Concept> result;
		 * 		vector<string> partial_stmts;
		 *
		 * 		YarpConnector connector("myDevice", "oro");
		 * 		oro = Ontology::createWithConnector(connector);
		 *
		 * 		partial_stmts.push_back("?mysterious oro:eats oro:banana_tree");
		 * 		partial_stmts.push_back("?mysterious oro:isFemale true^^xsd:boolean");
		 *
		 * 		oro->find("mysterious", partial_stmts, result);
		 * 
		 *		//display the results on std_out
		 * 		copy(result.begin(), result.end(), ostream_iterator<Concept>(cout, "\n"));
		 * 
		 * 		return 0;
		 * }
		 * \endcode
		 */
		void find(const std::string& resource, const std::vector<std::string>& partial_statements, std::vector<Concept>& result);

		void find(const std::string& resource, const std::string& partial_statement, std::vector<Concept>& result);


		/**
		 * Tries to approximately identify an individual given a set of known statements about this resource.\n
		 * Wrapper for server's \p laas.openrobots.ontology.OpenRobotsOntology#guess(String,Vector,double) method.\n
		 *
		 * Working code snippet:\n
		 *
		 * \code
		 * #include "oro.h"
		 * #include "yarp_connector.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 * 		vector<string> result;
		 * 		vector<string> partial_stmts;
		 *
		 * 		YarpConnector connector("myDevice", "oro");
		 * 		oro = Ontology::createWithConnector(connector);
		 *
		 * 		partial_stmts.push_back("?mysterious age \"40\"^^xsd:int");
		 * 		partial_stmts.push_back("?mysterious weight \"60\"^^xsd:double");
		 *
		 * 		oro->guess("mysterious", 0.8, partial_stmts, result);
		 * 
		 *		//display the results on std_out
		 * 		copy(result.begin(), result.end(), ostream_iterator<string>(cout, "\n"));
		 * 
		 * 		return 0;
		 * }
		 * \endcode
		 */
		int guess(const std::string& resource, const double threshold, const std::vector<std::string>& partial_statements, std::vector<std::string>& result);

		/**
		 * Performs a SPARQL query on the OpenRobots ontology.\n
		 * This method can only have one variable to select.\n
		 *\n
		 * Please note that resources within the SPARQL request must have their namespace specified (either the complete URI or the short one). Common prefixes are included. More can be specified at the server level.\n
		 * 
 		 * Working code snippet:\n
		 *
		 * \code
		 * #include "oro.h"
		 * #include "yarp_connector.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 * 		vector<string> result;
		 * 
		 *		YarpConnector connector("myDevice", "oro");
		 *		oro = Ontology::createWithConnector(connector);
		 * 
		 * 		oro->query("instances", "SELECT ?instances \n WHERE { \n ?instances rdf:type owl:Thing}\n, result);
		 * 
		 *		//display the results on std_out
		 * 		copy(result.begin(), result.end(), ostream_iterator<string>(cout, "\n"));
		 * 
		 * 		return 0;
		 * }
		 * \endcode
 		*/
		int query(const std::string& var_name, const std::string& query, std::vector<std::string>& result);
		
		/**
		 * Returns the set of asserted and inferred statements whose the given node is part of. It represents the "usages" of a resource.\n
		 * Usage example:\n
		 * \code

		 * \endcode
		 * 
		 * This example would print all the types (classes) of the instance ns:individual1.
		 * 
		 * @param resource the lexical form of an existing resource.
		 * @param result a vector of string related to the resource.
		 * @throw ResourceNotFoundOntologyException thrown if the resource doesn't exist in the ontology.
		 * @throw OntologyServerException thrown an error occured on the server during the query processing.
		 */
		void getInfos(const std::string& resource, std::vector<std::string>& result);
				
		/** Subscribe to a specified event in the ontology.\n
		 * 
		 * Working code snippet:\n
		 *
		 * \code
		 * #include "oro.h"
		 * #include "yarp_connector.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 * 		vector<string> result;
		 * 
		 *		YarpConnector connector("myDevice", "oro");
		 *		oro = Ontology::createWithConnector(connector);
		 * 
		 * 		oro->subscribe("?object rdf:type Bottle", ON_TRUE_ONE_SHOT, "myDevice_events");
		 * 		return 0;
		 * }
		 * \endcode
		 * 
		 * \param watchExpression a partial statement used as a pattern by the ontology server to trigger the event.
		 * \param triggerType the way the event is triggered. Cf \link EventTriggeringType the EventTriggeringType enum documentation \endlink for the list of available types.
		 * \param portToTrigger a string defining a port the ontology server should trigger when the expression to watch becomes true. What "port" means depends on the underlying implementation (YARP, Genom, ROS...).
		*/
		void subscribe(const std::string& watchExpression, EventTriggeringType triggerType, const std::string& portToTrigger);
		
		/**
		* Saves the in-memory ontology model to a RDF/XML file.
		* 
		* @param path The path and name of the OWL file to save to (for instance \c ./ontos/saved.owl )
		*/
		void save(const std::string& path);
		
		/**
		 * Returns several statistics on the ontology server.
		 * 
		 * In this order,
		 * <ul>
		 * 	<li>the hostname where the server runs</li>
		 * 	<li>server uptime</li>
		 * 	<li>the current amount of classes in the ontology</li>
		 * 	<li>the current amount of instances in the ontology</li>
		 * 	<li>the current amount of client connected to the server</li>
		 * </ul>
		 * 
		 * \param stats a vector of strings containing the various statistics.
		 */
		void stats(std::vector<std::string>& stats);
		
		/**
		* Generate a new random id which can be used to name new objects. Attention! no check for collision!
		*
		* @param length the length of the id. Default is 8 characters.
		*/
		static std::string newId(int length = 8);
		
		/**
		 * Enable the bufferization of queries to the ontology server. All subsequent request involving statement manipulation (like "add", "remove". It includes concept creation and manipulation) will be stored and retained until a call to {@link #flush()}.
		 * 
		 * Using bufferization can dramatically improve the performance since the call to the server are concatained. For instance:
		 * 
		 * \code
		 * #include "oro.h"
		 * #include "oro_library.h"
		 * #include "oro_connector.h"
		 * #include "yarp_connector.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 *
		 * 		//Create a connector to the ontology server.
		 *		YarpConnector connector("myDevice", "oro");
		 *	
		 *		//Instanciate the ontology with this connector.
		 *		oro = Ontology::createWithConnector(connector);
		 *
		 * 		oro->bufferize();
		 * 
		 * 		oro->add("gorilla rdf:type Monkey");
		 * 		oro->add("gorilla age 12^^xsd:int");
		 * 		oro->add("gorilla weight 75.2");
		 * 
		 * 		oro.flush(); //here, the 3 "add" requests will be actually send in one "add" with 3 statements.
		 *
		 * 		return 0;
		 * }
		 * \endcode
		 */
		void bufferize();
		
		 /**
		  * If buffering is enabled (cf {@link #bufferize()} ), optimize the buffer by concatenating what requests, actually send the requests, and flush the buffer.
		  */
		void flush();


	protected:
		IConnector& _connector;
		Ontology(IConnector& connector);
	private:
		void addToBuffer(const std::string, const Statement&);
		
		static Ontology* _instance;
		
		bool _bufferize;
		
		/**hold the number of "on-going" bufferization operation. It allows to flush the buffer only at the end of the "stack".
		 */
		int _buf_op_counter; 
		
		typedef std::map<std::string, Statement> BufStatements;
		std::map<std::string, BufStatements > _buffer;
		
	
};

/** This represents a class of the OpenRobots ontology.\n
 * Strictly speaking (and as assumed in OWL Full for instance), a class is a kind of concept. However, since we stay in OWL DL, for simplicity and clarity, classes and concepts won't overlap in this API.\n
 * 
 * You can easily use classes defines in your ontology
 * 
 */
class Class {
	public:
		
		/**
		 * Create a metaclass instance (ie, a class) from its literal name, in the default namespace (oro namespace).
		 * @param name the literal name of the class.
		 * @throw ResourceNotFoundOntologyException when the name can not be matched to a class name defined in the ontology.
		 */
		Class(const std::string& name);
		
		virtual ~Class();
		
		const std::string& name() const {return _name;}
		
		/**
		 * Return a computer-friendly string describing the class.
		 */
		const std::string& to_string() const {return _name;}
		
		/**
		 * Print, in a computer-friendly way, the class.
		 */
		friend std::ostream& operator<<(std::ostream& stream, const Class& c){
			stream<<c.name();
			return stream;
		}
		
	protected:
		std::string _name;
};

/** This represents a property (or predicate) of the OpenRobots ontology.
 */
class Property {
	public:
		/**
		 * Create a metaproperty instance (ie, a property) from its literal name, in the default namespace (oro namespace).
		 * \param name the literal name of the property.
		 * \throw ResourceNotFoundOntologyException when the name can not be matched to a property name defined in the ontology.
		 */
		Property(const std::string& name);
		
		virtual ~Property();
		
		/**
		 * Return the name of the property
		 */
		const std::string& name() const {return _name;}
		
		/**
		 * Return, in a computer-friendly way, the property id. Does currently the same as Property.name().
		 */
		const std::string& to_string() const {return _name;}
		
		/**
		 * Print, in a computer-friendly way, the property.
		 */
		friend std::ostream& operator<<(std::ostream& stream,const Property& p){
			stream<<p.name();
			return stream;
		}
		
	protected:
		std::string _name;
};

/** This represents a concept (an instance or an individual in OWL terminology) of the OpenRobots ontology.
 */
class Concept {
	public:
		/**
		 * Constructs a new, under-specified instance (actually an instance of owl:Thing) associated to a random identifier.
		 */
		Concept();
		
		/**
		 * Constructs a object from a previous identifier. From a semantic point of view, the new object is strictly equal to the concept whose identifier is passed as parameter.
		 */
		Concept(const std::string& id);
		
						
		static Concept create(const std::string& label);
		
		static Concept create(const std::string& label, const Class& type);
		
		static Concept create(const Class& type);
				
		/**
		 * This is a special member of the Concept class representing the semantic of the "nothing" concept. It's the unique, virtual, instance of the class Nothing.
		 * \see Class::Nothing
		 */
		static const Concept nothing;

		/**
		 * Adds a new assertion regarding the current concept to the ontology.\n
		 * It adds the triple \p [this_object] \p [the_predicate] \p [the_value] to the ontology.
		 * 
		 * \param predicate the predicate of the statement.
		 * \param value the literal representation of the object of the statement.
		 * \throw ResourceNotFoundOntologyException when the predicate name can not be matched to a property name defined in the ontology.
		 * \throw OntologySemanticException if the addition of a statement causes the ontology to become inconsistent.
		 * \see remove
		 */
		void assert(const Property& predicate, const std::string& value);
		
		/**
		 * Adds a new assertion regarding the current concept to the ontology.\n
		 * It adds the triple \p [this_object] \p [the_predicate] \p [the_value] to the ontology.
		 * 
		 * \param predicate the predicate of the statement.
		 * \param value the object of the statement.
		 * \throw ResourceNotFoundOntologyException when the predicate name can not be matched to a property name defined in the ontology.
		 * \throw OntologySemanticException if the addition of a statement causes the ontology to become inconsistent.
		 * \see remove
		 */
		void assert(const Property& predicate, const Concept& value);

		/**
		 * Removes all the assertions in the ontology which have the current concept as subject and the given property as predicate.\n Assertions with subproperties as predicate are not removed.
		 * \param predicate the property whose assertions are to be removed.
		 */
		void clear(const Property& predicate);
		
		/**
		 * Returns whether the object has or not some property, ie at least one assertion involves the current concept as subject, and the property (or any subproperty) you want to check as predicate.\n
		 * For instance,
		 * \code
		 * Agent robot = Agent::create(Classes::Robot);
		 * robot.has(Properties::isAt);
		 * \endcode
		 * 
		 * \param predicate the property you want to check.
		 * \return \p true or \p false if it's possible to infer the answer, \p indeterminate in other cases.
		 */
		boost::logic::tribool has(const Property& predicate) const;
		boost::logic::tribool has(const Property& predicate, const std::string& value) const;
		boost::logic::tribool has(const Property& predicate, const Concept& value) const;

		/**
		 * Returns all the objects that are bound to the current concept through the given predicate (either directly asserted or inferred).
		 * 
		 * For instance,
		 * \code
		 * #include "oro.h"
		 * #include "yarp_connector.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 * 
		 *	YarpConnector connector("myDevice", "oro");
		 *	oro = Ontology::createWithConnector(connector);
		 *
		 *	Agent myself("myself");
		 *	Concept human = new Concept(Classes::Human);
		 *
		 *	human.sees(myself);
		 * 
		 * 	set<concept> whatTheHumanSees = human.getObjectForPredicate(Properties::sees);
		 *	if(whatTheHumanSees.find(myself) != whatTheHumanSees.end())
		 *	{
		 *   	//The human sees me...
		 * 	}
		 * \endcode
		 * 
		 * \param predicate the property you want to check.
		 * \return a set of concepts that are linked to the current concept by the given property.
		 */
		std::set<Concept> getObjectsForPredicate(const Property& predicate) const;
		
		/**
		 * Returns the ID of the concept. Beware: two different ID may refer to the same actual concept (OWL doesn't rely on the Unique Name Assumption).
		 * \return the ID of the concept.
		 */
		std::string id() const;

		/**
		 * Returns the status (true or false) of some boolean property.
		 * @param boolDataproperty 
		 * @return \p true of \p false if the property is set, \p indeterminate in other cases.
		 */
		boost::logic::tribool is(const Property& boolDataproperty) const;

		/**
		 * Checks the type (ie class) of a concept.
		 * \param type the class you want to check the concept against.
		 * \return \p true or \p false if it's possible to infer the answer, \p indeterminate in other cases.
		 */
		boost::logic::tribool isA(const Class& type) const {return hasType(type);}
		
		/**
		 * Alias for \p isA . 
		 * \param type the class you want to check the concept against.
		 * \return \p true or \p false if it's possible to infer the answer, \p indeterminate in other cases.
		 * \see{isA}
		*/
		boost::logic::tribool hasType(const Class& type) const;
		
		/**
		 * Set the class (or type) of a concept. If the class was previously already defined, this definition will be added.\n
		 * This method adds this assertion: \p [this_object] \p rdf:type \p [the_class] to the ontology.
		 * 
		 * \param type the class you want to check the concept against.
		 * \return \p true or \p false if it's possible to infer the answer, \p indeterminate in other cases.
		 */
		void setType(const Class& type);
		
		/**
		 * 
		 */
		const Class& type() const;

		/**
		 * Sets a human-readable label for this concept.\n
		 * This method sets the \p rdfs:label annotation for this concept.
		 * 
		 * \param label the human-readable name of the concept, in plain English.
		 */
		void setLabel(const std::string& label);
		
		/**
		 * Returns the human-readable name of the concept.
		 * 
		 * \return the human-readable form of the concept name, or an empty string if no label has been defined.
		 */
		std::string label() const;

		void remove(const Property& predicate, const std::string& value);		
		void remove(const Property& predicate, const Concept& value);
		

		/**
		 * Returns a computer-friendly string describing the concept.
		 */
		std::string to_string() const {return id();}
		
		/**
		 * Print, in a computer-friendly way, the concept.
		 */
		friend std::ostream& operator<<(std::ostream& stream,const Concept& c){
			stream<<c._id;
			return stream;
		}

	protected:
		std::string _id;
		std::string _label;
		

};


/** This represents an object (ie an instance of the "Object" class) of the OpenRobots ontology.\n
 * An object is a spacially localized and at least partially tangible kind of thing.\n
 * This class provides common, higher-level functionnalities over the base class \p Concept .\n
 * To create a new Agent instance, use the Concept::create<Object>() factory.
 */
class Object : public Concept {
	protected:
		Object() {};
	public:
		Object(const std::string& id) : Concept(id) {};
		
		static Object create();
		
		static Object create(const std::string& label);
		
		static Object create(const std::string& label, const Class& type);
		
		static Object create(const Class& type);
		
		boost::logic::tribool hasAbsolutePosition();
		
		/**
		 * Returns a concept which represent the location of the current object, as far as it can be inferred from the ontology.\n
		 * If no position has been asserted or can be inferred, \p Concept::nothing is returned.
		 * 
		 * \see Concept::nothing
		 */
		Concept hasPosition();
		
		void setColor(int hue);
		
		void setAbsolutePosition(double x, double y, double z);
};

//Forward declaration
class Action;

/** This represents an agent (ie an instance of the "Agent" class) of the OpenRobots ontology.\n
 * An agent is a special kind of object, endowed with some kind of intelligence. In the OpenRobots ontology, agents are either humans or robots.\n
 * This class provides common, higher-level functionnalities over the base classes \p Concept and \p Object \n
 * To create a new Agent instance, use the Concept::create<Agent>() factory.
 */
class Agent : public Object {
	protected:
		Agent() {};
	public:
		Agent(const std::string& id) : Object(id) {};
		
		static Agent create();
		
		static Agent create(const std::string& label);
		
		static Agent create(const std::string& label, const Class& type);
		
		static Agent create(const Class& type);
		
		/**
		 * This static field is always accessible and represent in the ontology "myself", ie the agent doing the reasonning.
		 */
		//static const Agent myself;
		
		void desires(const Action& action);
		void currentlyPerforms(const Action& action);
		void sees(const Concept& object);
};

//const Agent Agent::myself = Concept::create<Agent>("myself");

/** This represents an action (ie an instance of the "Action" class) of the OpenRobots ontology.\n
 * An action is a special kind of event, carried out by an agent.\n
 * This class provides common, higher-level functionnalities over the base class \p Concept .\n
 * To create a new Agent instance, use the Concept::create<Action>() factory.
 */
class Action : public Concept {
	protected:
		Action() {};
	public:
		Action(const std::string& id) : Concept(id) {};
		
		static Action create();
		
		static Action create(const std::string& label);
		
		static Action create(const std::string& label, const Class& type);
		
		static Action create(const Class& type);
		
		void object(const Concept& concept);
		void recipient(const Object& concept);
};

/** A statement is the atomic element of the ontology.\n
 * It is made of a triplet (subject, predicate, object).\n
 * While the subject and the predicate are respectively instances of Concepts and Properties, the object of the statement can be either a Concept or a literal.\n
 * 
 * See the classes Concept and Property for details regarding these objects.\n
 * 
 * You can refer to the SPARQL documentation (http://www.w3.org/TR/rdf-sparql-query/#QSynLiterals) to have an easy-to-read overview of the possible syntax for literals. Please refer as well to the \p oro-server documentation, here: \link https://www.laas.fr/~slemaign/doc/oro-server/laas/openrobots/ontology/backends/IOntologyBackend.html#createStatement(java.lang.String) IOntologyBackend::createStatement \endlink \n
 * 
 * Some examples of literals include:\n
 * \li "chat"
 * \li 'chat'\@fr with language tag "fr"
 * \li "xyz"^^<http://example.org/ns/userDatatype>
 * \li "abc"^^appNS:appDataType
 * \li '''The librarian said, "Perhaps you would enjoy 'War and Peace'."'''
 * \li 1, which is the same as "1"^^xsd:integer
 * \li 1.3, which is the same as "1.3"^^xsd:decimal
 * \li 1.300, which is the same as "1.300"^^xsd:decimal
 * \li 1.0e6, which is the same as "1.0e6"^^xsd:double
 * \li true, which is the same as "true"^^xsd:boolean
 * \li false, which is the same as "false"^^xsd:boolean
 */
class Statement {
	public:
		Concept subject;
		Property predicate;
		
		Concept object;
		std::string literal_object;
		
		bool isObjectLiteral;
		
		
		Statement(const Concept& subject, const Property& predicate, const Concept& object);
		Statement(const Concept& subject, const Property& predicate, const std::string& object);
		
		inline bool operator==(const Statement& stmt) const;
		
		/**
		 * Creates a new statement from its literal string representation.
		 * For details regarding the syntax, please refer to the Statement class main documentation page.
		 */
		static Statement create(const std::string& stmt);
		
		/**
		 * Returns a computer-friendly string describing the concept.
		*/
		std::string to_string() const;
		
		/**
		 * Print, in a computer-friendly way, the statement.
		 */
		friend std::ostream& operator<<(std::ostream& stream,const Statement& stmt){
			stream<<stmt.subject<<" "<<stmt.predicate<<" " << (stmt.isObjectLiteral?stmt.literal_object:stmt.object);
			return stream;
		}

		
};
}

#endif /* ORO_H_ */
