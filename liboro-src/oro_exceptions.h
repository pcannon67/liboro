/*
 * ©LAAS-CNRS (2008-2009)
 *
 * contributor(s) : Séverin Lemaignan <severin.lemaignan@laas.fr>
 *
 * This software is a computer program whose purpose is to interface
 * with an ontology server in a robotics context.
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

/** \file
 * This header defines some C++ exceptions specific to \p liboro .
 */

#ifndef ORO_EXCEPTIONS_H_
#define ORO_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace oro {

	/** The string identifying a "NotFoundException" as emitted by the ontology server.
	 */	
	static const std::string SERVER_NOTFOUND_EXCEPTION = "com.hp.hpl.jena.shared.NotFoundException";
	
	/** The string identifying a "QueryParseException" as emitted by the ontology server.
	 */	
	static const std::string SERVER_QUERYPARSE_EXCEPTION = "com.hp.hpl.jena.query.QueryParseException";

/*********************************************************
 *                      Exceptions                       *
 *********************************************************/

/**
 * Generic exception for everything related to the handling of ontologies. Does not include errors related to the server itself.
 */
class OntologyException : public std::runtime_error {
	public:
		OntologyException() : std::runtime_error("An exception occured related to the ontology.") { }
		OntologyException(const char* msg) : std::runtime_error(msg) { }
		OntologyException(const std::string& msg) : std::runtime_error(msg.c_str()) { }
};

/**
 * Generic exception for every server-related exceptions. Does not include errors related to the ontology reasonning.
 */
class OntologyServerException : public std::runtime_error {
	public:
		OntologyServerException() : std::runtime_error("OntologyServerException") { }
		OntologyServerException(const char* msg) : std::runtime_error(msg) { }
		OntologyServerException(const std::string& msg) : std::runtime_error(msg.c_str()) { }
};

/**
 * Exception for every conenctor-related problems.
 */
class ConnectorException : public std::runtime_error {
	public:
		ConnectorException() : std::runtime_error("ConnectorException") { }
		ConnectorException(const char* msg) : std::runtime_error(msg) { }
		ConnectorException(const std::string& msg) : std::runtime_error(msg.c_str()) { }
};

/**
 * Generic exception when the ontology loose its consistency (<=> semantic error).
 */
class OntologySemanticException : public OntologyException {
	public:
		OntologySemanticException() : OntologyException("An consistency exception occured on the ontology.") { }
		OntologySemanticException(const char* msg) : OntologyException(msg) { }
};

/**
 * Thrown if two statements are to be asserted for the same (subject, predicate) couple when the predicate is functional.
 */
class FunctionalPredicateException : public OntologySemanticException {
	public:
		FunctionalPredicateException() : OntologySemanticException("Error: two statements involving the same functional predicate!") { }
		FunctionalPredicateException(const char* msg) : OntologySemanticException(msg) { }
};

/**
 * Thrown if an individual is set to have two different types (classes) which are disjoint.
 */
class DisjointTypesException : public OntologySemanticException {
	public:
		DisjointTypesException() : OntologySemanticException("An individual may not have two disjoint types.") { }
		DisjointTypesException(const char* msg) : OntologySemanticException(msg) { }
};

/**
 * Thrown by the method Concept::is() if the requested property is not with a boolean range.
 */
class NonBooleanDatatypePropertyException : public OntologyException {
	public:
		NonBooleanDatatypePropertyException() : OntologyException("The datatype property invoked from a Concept.is(property) is not boolean.") { }
		NonBooleanDatatypePropertyException(const char* msg) : OntologyException(msg) { }
};

/**
 * Thrown if ontology is accessed but not yet initialized.
 */
class UninitializedOntologyException : public OntologyException {
	public:
		UninitializedOntologyException() : OntologyException("Ontology not yet initialized! you must specify a port name and a server name!") { }
		UninitializedOntologyException(const char* msg) : OntologyException(msg) { }
};

/**
 * Thrown when a resource is referenced but not found (a wrong class or property name, for instance).
 */
class ResourceNotFoundOntologyException : public OntologyException {
	public:
		ResourceNotFoundOntologyException() : OntologyException("A resource was not found. Wrong class or property name?") { }
		ResourceNotFoundOntologyException(const char* msg) : OntologyException(msg) { }
		ResourceNotFoundOntologyException(const std::string& msg) : OntologyException(msg.c_str()) { }
};

/**
 * Thrown when a query (currently, SPARQL) is not valid (not parsable -> syntax error).
 */
class InvalidQueryException : public OntologyException {
	public:
		InvalidQueryException() : OntologyException("Invalid query. Check your syntax!") { }
		InvalidQueryException(const char* msg) : OntologyException(msg) { }
		InvalidQueryException(const std::string& msg) : OntologyException(msg.c_str()) { }
};

/**
 * Thrown when a statement is not valid (for instance, contain only 2 tokens).
 */
class InvalidStatementException : public OntologyException {
	public:
		InvalidStatementException() : OntologyException("A statement must contain precisely 3 tokens (the subject, the predicate and the object)") { }
		InvalidStatementException(const char* msg) : OntologyException(msg) { }
};

}

#endif /* ORO_EXCEPTIONS_H_ */
