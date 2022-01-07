/*! \mainpage Project ANVIL: Drone Delivery System
 *\section uml_sec UML Diagram Iteration 2:
 *\image html uml.png width=100% height=100%
 * \section intro_sec The System
 * This drone delivery system allows customers both to receive the packages they require and
 * to ship the packages they want to send. Even better, the drone delivery process can be
 * watched in real time online!
 *
 * The project incorporates read-only Libraries provided by the instructors. Among what's
 * provided would include the DroneDeliverySystem interface (from which DroneSimulation
 * inherits), the Entity class (which is the base class for Drone, Customer, and Package),
 * the EntitySystem class (which facilitates simulation by allowing events to occur over
 * time), and the WebSceneViewer class (which allows the simulation to be viewed in 3D in
 * a web browser).
 *
 * DroneSimulation is the class that implements the methods of DroneDeliverySystem. It is the
 * entryway into the subsystems of the facade like EntityFactory and TransportPackage. It is
 * responsible for bringing subsystems together to make a cohesive program.
 *
 * EntityFactory is responsible for creating Entity objects that are rendered in the simulation.
 * TransportPackage is responsible for giving Drone the routes it needs to follow to the
 * Package and Customer, orchestrating the pickup/dropoff, and flying the Drone.
 *
 * Refer to the Design Document for the final UML diagram of the system.
 *
 * \subsection run_info Build and Run
 *
 * 1. In Vole, navigate to the project directory.
 * 2. Use the make command to build the project.
 * 3. Run the project using the command ./bin/run.sh
 * 4. Open Firefox and type "http://127.0.0.1:8081" into the URL bar.
 *
 *
 */
