/*!
 * \defgroup SAFE-DART-Framework SAFE-DART Framework
 *
 * \brief A framework allowing components of a software system to be changed at run-time via a
 * configuration file.
 *
 * The Software Architecture For Extensibility using Dynamic Assembly of Reflective Types
 * (SAFE-DART) is a framework which uses the reflection functionality provided by Qt to assemble the
 * components of a software system based on the name of the component to use, specified in a
 * configuration file. This allows for software components to be selected based on the scenario at
 * hand.
 *
 * In addition to allowing for run-time configuration, SAFE-DART also provides the ability to load
 * additional types from modules, stored as shared libraries (SO or DLL files). This is the source
 * of the extensibility provided by SAFE-DART. Variations of software components can be added to the
 * software system via a shared library, and components provided by that library are available for
 * use without the need for any code changes to the original system in any way except its
 * configuration file.
 */
