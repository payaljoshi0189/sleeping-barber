code Main

  -- OS Class: Project 3
  --
  -- <Payal Pradeep Joshi>
  --

-----------------------------  Main  ---------------------------------

  function main ()
      print ("Assignment 3")
      nl()
      InitializeScheduler ()
      
      SleepingBarber()

      ————————————————GamingParlour()

      ThreadFinish()

    endFunction


----------------------------------------------------------------Sleeping Barber Problem ---------------------------------------------------------------------------------------------------
   --This solution attempts to solve the sleeping barber problem using semaphore approach,
   --and prints the output for each customer threads and a barber thread
   --Following functions are defined for this:
   --	SleepingBarber(): 
   --		This function initializes the semaphores and customer and barber threads
   --		Customer thread calls Customer() method and Barber calls Barber() method 
   --		on Fork.
   --	Barber(id):
   --		This method checks if any customers are available by doing a down on customer
   --		semaphore. If none, then sleep. If available, decrease the count of waiting customers
   --		,modify the chair status,set its own status to start and calls Cuthair()
   --  Customer(id):
   --		This method is run for all the customer threads. Initially, a check is done if the
   --		any chair is available for waiting, if no then customer leaves, if yes then customer 
   --		sits on the waiting chair and calls Gethaircut() method     
   --  Cuthair(): 
   --		This function performs a Yield() on the barber thread for 100 times which is treated as if
   --		barber is cutting hair and then barber changes its status to end
   --  Gethaircut():
   --		This function sets the calling customer thread's status to Begin haircut 'B' and then
   -- 		yields in loop for 50 times which is treated as if customer is getting hair cut. Lastly
   --		It sets its status to finish haircut 'F' and loops for another 50 count before finally 
   --		setting the status to leave 'L'
   --  PrintAllStatus():
   --		This function prints the status of all the chairs, the barber and all the customer threads
   --		It is called everytime any thread changes its status







 enum E, S, B, F,L								--Customer Status
 enum X, N									--Chair Status
 enum start,end									--Barber Status
 const
    CHAIRS = 5
 var
     waiting: int = 0                                   			--Number of customers waiting
     semaphoreBarber: Semaphore = new Semaphore					--Semaphore for barber
     semaphoreCustomer: Semaphore = new Semaphore       			--Semaphore for Customer
     semaphoreMutex: Semaphore = new Semaphore					--Semaphore for Mutex
     barber: int = 0								--Semaphore Count for barber
     customer: int = 0							       	--Semaphore Count for customer
     mutex: int = 1							       	--Semaphore Count for mutex lock
     thArray: array [10] of Thread = new array of Thread { 10 of new Thread }  	--Customer Thread list
     barberThread: Thread = new Thread					       	--Barber thread
     chairStatus: array [5] of int = new array of int { 5 of N } 	       	--Chair status array list, initially initialized to N
     customerStatus: array[10] of int = new array of int { 10 of E }		--Customer status array list, initially initialized to E
     chairStatusCount: int = 0						       	--Pointer for chair status array
     barberStatus: int = start							--status for barber




  function SleepingBarber()
     
     print("         Barber    1 2 3 4 5 6 7 8 9 10")
     nl()

     --Initialize all the semaphores
     semaphoreBarber.Init(barber)
     semaphoreCustomer.Init(customer)
     semaphoreMutex.Init(mutex)

     --Initialize Barber threads
     barberThread.Init( "10" )
     barberThread.Fork(Barber,10)	

     --Initialize Customer threads
     thArray[0].Init("0")
     thArray[0].Fork(GoToShop,0)
     
     thArray[1].Init("1")
     thArray[1].Fork(GoToShop,1)

     thArray[2].Init("2")
     thArray[2].Fork(GoToShop,2)
 
     thArray[3].Init("3")
     thArray[3].Fork(GoToShop,3)
    
     thArray[4].Init("4")
     thArray[4].Fork(GoToShop,4)
     
        
     thArray[5].Init("5")
     thArray[5].Fork(GoToShop,5)

     thArray[6].Init("6")
     thArray[6].Fork(GoToShop,6)


     thArray[7].Init("7")
     thArray[7].Fork(GoToShop,7)

     thArray[8].Init("8")
     thArray[8].Fork(GoToShop,8)
  
     thArray[9].Init("9")
     thArray[9].Fork(GoToShop,9)

   endFunction

  function GoToShop(threadNum: int)
   var 
    i: int
    j: int
   
   --Each customer Enters the shop only once
   --Tested the code by changing the looping value
   --so that each customer enters the shop
   --i number of times
   for i = 1 to 1
     Customer(threadNum)
      for j = 1 to 50
       currentThread.Yield()
      endFor
   endFor

  endFunction 
  
  function Barber( myId: int )
     var
       j: int				
    
      while(true)
       semaphoreCustomer.Down()                          --If no customer is available,barber sleeps
       semaphoreMutex.Down()				--Acquire mutex lock
       waiting = waiting - 1                             --Decrementing the waiting customer count

       --Before waking up the customer thread from sleep, update the barberstatus to start
       barberStatus = start
     
       --Update the chair status, i.e remove the waiting customer from the chair
       for j = 0 to 3
        chairStatus[j] = chairStatus[j+1]
       endFor

       chairStatus[4] = N
       chairStatusCount = chairStatusCount - 1 
 
       --Print all the statuses
       PrintAllStatus ( 10 )
      
       semaphoreBarber.Up()				--Barber is ready to cut the hair
       semaphoreMutex.Up()				--Release the lock
      
       Cuthair()
     

      endWhile
  
  endFunction



  function Customer( myId: int )
    
    customerStatus [ myId ] = E      			--Once a new customer comes in , change the customer's status

    semaphoreMutex.Down()				--Acquire the lock
    PrintAllStatus( myId )				--Print the status

    if waiting < CHAIRS					--Check if the number of waiting customers is less than the available chairs
      waiting = waiting + 1				--Increment the number of waiting customer's count
      chairStatus[chairStatusCount] = X                 --Acquire the chair, change the chair status
      customerStatus [ myId ] = S			--Change the customer status to S
      PrintAllStatus( myId )				--Print the status after acquiring the chair
      chairStatusCount = chairStatusCount + 1		--Increase the count of chairStatus array
      semaphoreCustomer.Up()				--Increment the count for Customer semaphore
      semaphoreMutex.Up()				--Release the lock
      semaphoreBarber.Down()				--If barber is not available, then sleep
      Gethaircut( myId )			        --Get haircut
    else
      customerStatus [ myId ] = L                       --If no chair is available, customer leaves, change its status to L
      PrintAllStatus( myId )
      semaphoreMutex.Up()				--If number of waiting customers is greater than the chairs, then release the lock and leave
    endIf
  
   endFunction



  function Cuthair()
    var 
     i: int 
     
     --Cut Hair
     for i = 1 to 100
       currentThread.Yield()				--Yield to give customer threads opportunity to enter the shop
     endFor		


    --After the hair cut, set barber status to end,Print all the statuses
    semaphoreMutex.Down()
    barberStatus = end
    PrintAllStatus(10)
    semaphoreMutex.Up()
   endFunction



  
 function Gethaircut( customerNum: int )
   var
    i: int
    j: int

    --Acquire the lock,set the status for current customer to Begin Haircut,print the status
     customerStatus [ customerNum ] = B
     semaphoreMutex.Down()
     PrintAllStatus ( customerNum ) 
     semaphoreMutex.Up()
     

    --Get Haircut
    for i = 1 to 50
     currentThread.Yield()				
    endFor
   
   --Acquire the lock,set the status to Finish Haircut,print the status
   customerStatus [ customerNum ] = F

   semaphoreMutex.Down()
   PrintAllStatus ( customerNum )
   semaphoreMutex.Up ()

   --After the hair cut, change customer status to L
   customerStatus [ customerNum ] = L
    
   for j = 1 to 50
    currentThread.Yield()				--Yield to let other customers and barber thread run
   endFor
   

   semaphoreMutex.Down()
   PrintAllStatus ( customerNum )
   semaphoreMutex.Up()
 
  endFunction


 function PrintAllStatus(threadNum: int)
   var
    i: int
    j: int

   --print the chair status of each chair  
   for i = 0 to 4
   switch chairStatus[i]
    case X:
     printChar('X')
     break
    case N:
     printChar('_')
     break
    endSwitch
   endFor
   
   --if thread is barber, print status after 9 paddings
    
   if threadNum == 10					--Print Barber's status
     
     print("    ")

     switch barberStatus
      case start:
       print("start")
       break
      case end:
       print("end")
       break
     endSwitch

     nl()

   else							--Print Customer Status
     
     --Padding after printing chair status till the 1st customer is 13 spaces
     print("             ")

     --Padding before printing the customer status
     for j = 0 to threadNum * 2
      print(" ")
     endFor

  
     switch customerStatus [threadNum]
      case E:
       print("E")
       break
      case S:
       print("S")
       break
      case B:
       print("B")
       break
      case F:
       print("F")
       break
      case L:
       print("L")
       break 
     endSwitch

     nl()

   endIf

  endFunction


------------------------------------- The Gaming Parlor Problem ------------------------------------------------
 --enum PLAYING, NOTPLAYING
  var
   mon: GameParlor
   
  --Define array of Thread having size 8 representing 8 groups
   groups : array [8] of Thread = new array of Thread { 8 of new Thread }

  
  --This function initializes all the group threads and fork them using FrontDesk() method
   function GamingParlour ()
				
    mon = new GameParlor 					--Initialize Monitor
    mon.Init()
 								--Initialize all the group threads
    groups[0].Init ("A")
    groups[0].Fork (FrontDesk,4)
   
    groups[1].Init ("B")
    groups[1].Fork (FrontDesk,4)

    groups[2].Init ("C")
    groups[2].Fork (FrontDesk,5)

    groups[3].Init ("D")
    groups[3].Fork (FrontDesk,5)

    groups[4].Init ("E")
    groups[4].Fork (FrontDesk,2)

    groups[5].Init ("F")
    groups[5].Fork (FrontDesk,2)

    groups[6].Init ("G")
    groups[6].Fork (FrontDesk,1)
  
    groups[7].Init ("H")
    groups[7].Fork (FrontDesk,1)

   endFunction


   --This function takes number of dice required by each group as argument
   --It calls Request() method of monitor to request the number of dice required by the calling group
   --After the group finishes the game, calls Return() method of monitor to return the dice
   function FrontDesk (numberOfDice: int)
     var
      i: int

      for  i= 0 to 4
     
      --Request for Dice
       mon.Request (numberOfDice)

      --Play game
       currentThread.Yield ()
     
      --Return the dice
       mon.Return (numberOfDice)          
       currentThread.Yield()					--Yield after releasing the dice so other threads get an opportunity to request the dice      
     endFor
   endFunction









-----------------------------------------------------------------------GameParlor---------------------------------------------------
   --This behavior defines following methods and variables:
   --monitorMutex: This is a monitor mutex which is acquired at the beginning of each entry methods and released at the end
   --waitingGrpOnDiceCondition: 
   --                      This condition variable of monitor is maintained for the group that requested the dice
   --		           first. It makes sure that the thread that requested the dice first is not starved for
   --		           other threads with smaller dice requirements
   --otherGrpsWaitingOnDiceCondition: 
    --			   This condition variable maintains the waiting list of all the threads that are waiting 
   --		           for dice to become available.
   --numberDiceAvail: This global variable is used to represent number of available dice, initialized to 8 initially
   --waitingGroups: Gloabl variable to maintain the count of groups requesting the dice
   --Init():
   --	 This method initializes the mutex lock and condition variables for monitor
   --Print():
   --    This method is called when a group requests dice,after the request is processed and once the group returns the dice.
   --	 It also prints the number of dice available
   --Request(numberOfDice):
   --	 Each group calls this entry method of monitor to request for required number of dice. If the required number of dice
   --    are available, the request is processed else the thread waits on monitor's condition variable
   --Return(numberOfDice):
   --	 After the group thread finishes playing, it calls monitor's Return() method to return all the acquired dice. After any group returns the dice
   --	 it sends a signal to the thread waiting on monitor's condition variable firstWaitingGrpOnDiceCondition 

  class GameParlor
   superclass Object

   fields
     monitorMutex: Mutex
     waitingGrpOnDiceCondition: Condition
     otherGrpsWaitingOnDiceCondition: Condition
     numberDiceAvail: int
     waitingGroups: int

   methods
     Init ()
     Print (str: String,count: int)
     Request (numberOfDice: int)
     Return (numberOfDice: int)
   endClass


  behavior GameParlor

   method Init()

    monitorMutex = new Mutex
    monitorMutex.Init()						--Initialize monitor's mutex

    waitingGrpOnDiceCondition = new Condition				
    waitingGrpOnDiceCondition.Init()                            --Initialize the condition variable
    
    otherGrpsWaitingOnDiceCondition = new Condition
    otherGrpsWaitingOnDiceCondition.Init()


    numberDiceAvail = 8						--Initialize the number of available dice to 8

   endMethod


   method Request (numberOfDice: int)
    monitorMutex.Lock ()					--Acquire mutex lock, if not available, calling thread will be put on monitorMutex's waiting thread list
    
    self.Print ("requests", numberOfDice)			--Print how many number of dice are requested by calling thread
    
    waitingGroups = waitingGroups + 1				--After the group requests the dice, count the group in the waiting group trying to acquire the dice

     if (waitingGroups > 1)					--If there are already groups that are waiting on the dice to be available, then add the calling thread to the
      otherGrpsWaitingOnDiceCondition.Wait(&monitorMutex) 	-- waitlist of this condition
    endIf

    
    --After the thread from above condition is signalled
    -- and  becomes ready,
    --it checks if sufficient dice are available for it
    --to proceed, if no, then it waits on
    --waitingGrpOnDiceCondition condition variable
    --This makes sure that, once dice are available
    --it will be the 1st thread to run, hence
    --avoiding starvation

    while(numberOfDice >  numberDiceAvail)
	waitingGrpOnDiceCondition.Wait (&monitorMutex)	
    endWhile
    
    waitingGroups = waitingGroups - 1				--After acquiring the dice, remove from waitingGroups

    otherGrpsWaitingOnDiceCondition.Signal(&monitorMutex)	--Signal the group waiting on this condition variable, so that it can check if the dice are available

    numberDiceAvail = numberDiceAvail - numberOfDice
    
    self.Print ("proceeds with", numberOfDice)

    monitorMutex.Unlock ()					--Release the lock before returning
   endMethod


   
   method Return (numberOfDice: int )
    monitorMutex.Lock ()					--Acquire mutex lock, if not available, calling thread will be put on monitorMutex's waiting thread list
    
    numberDiceAvail = numberDiceAvail + numberOfDice		--Add the number of dice returned to number of dice available global variable
    
    self.Print ("releases and adds back", numberOfDice)	 	--Print how many dice are returned by the calling group

    waitingGrpOnDiceCondition.Signal (&monitorMutex)	        --After the dice are returned, Send a signal to the group which is waiting on the waitingGrpOnDiceCondition variable
  
    monitorMutex.Unlock()					--Release the lock before returning
   endMethod




  method Print (str: String, count: int)
    -- This method prints the current thread's name and the arguments. 
    -- It also prints the current number of dice available.
       print (currentThread.name)
       print (" ")
       print (str)
       print (" ")
       printInt (count)
       nl ()
       print ("------------------------------Number of dice now avail = ") printInt (numberDiceAvail)
       nl ()
  endMethod

 endBehavior

endCode

