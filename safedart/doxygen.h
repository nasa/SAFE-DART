/*!
 * \defgroup SAFE-DART-Host SAFE-DART Host Executable
 *
 * \brief The host executable for a software system constructed using the SAFE-DART framework.
 *
 * By providing an implementation of the Application interface, a software system may be hosted by
 * the \c safedart executable. This simplifies the application of SAFE-DART's functionality by
 * eliminating the need to create a Builder or load modules inside the software system. In addition,
 * encapsulating the software system's \c main function inside of an object which is created with an
 * existing Builder object simplifies unit testing of the software system's \c main function, as
 * unit tests can provide an instance of Builder which contains all appropriate mock objects.
 *
 * The host executable can be invoked as follows:
 *
\code{.sh}
safedart -f path/to/config.ini -s section ApplicationImpl
safedart --file path/to/config.ini --section section ApplicationImpl
\endcode
 *
 * The command lines above will both load configuration data from the file located at
 * "path/to/config.ini", using the section named "section", and attempt to run an application named
 * "ApplicationImpl". The flags specifying the config file and section are optional; they will
 * default to "safedart.ini" and "safedart" respectively.
 *
 * Within the specified section of the specified configuration file, \c safedart will use the
 * following configuration keys:
 *
 * \li \@module_dirs - A comma-separated list of directories from which to load modules. Relative to
 * the working directory.
 * \li \@module_files - A comma-separated list of module files to load. Relative to the working
 * directory.
 */
