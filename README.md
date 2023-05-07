# Smart-Locker-System
## 1. Steps For Unlocking 
 1. The RFID Card is brought near to the RC522 RFIC reader.
 2. If your card is correct, you will receive an SMS on the registerd number stating **Enter Password**.
 3. Send your password through SMS to the same number from which you received your Enter Passsword message(locker's number).
 4. If your password is correct, the servo motor will move giving you the access of the Locker.
 
 ## 2. Steps For Locking
 1. Considering your locker is now in open state (all 4 steps of unlocking have been performed), you will receive an SMS from the locker stating **Enter password to lock again**.
 2. Send your password through SMS to the locker's number.
 3. If your password is correct the locker will again get locked and your items will be secured.
 
 ## Anti-Theft Feature
 There is a piezoelectric sensor attached to the locker for the Anti-Theft feature. Whenever someone tries to break the locker or move it from its original position, the locker will detect the vibration and send SMS to the owner stating **Locker under threat**. Since SMS is tend to be ignored sometimes, the locker will subsequently call the owner to aware him about the emergency situation.
 
