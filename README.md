Description:
Given the information in the joint timetable, write a program to produce two modified timetables, one for Posh Bus Company and one for Grotty Bus Company, each satisfying the following requirements:
1. All entries in each timetable are in order of departure time.
2. Any service longer than an hour shall not be included.
3. Only efficient services shall be added to the timetable. A service is considered efficient compared to the other one:
- If it starts at the same time and reaches earlier, or
- If it starts later and reaches at the same time, or
- If it starts later and reaches earlier.
4. If both companies offer a service having the same departure and arrival times then always choose Posh Bus Company over Grotty Bus Company, since Grotty Bus Company busses are not as comfortable as those of Posh Bus Company.

Example input and output.
Given the following data:
Posh 10:15 11:10
Posh 10:10 11:00
Grotty 10:10 11:00
Grotty 16:30 18:45
Posh 12:05 12:30
Grotty 12:30 13:25
Grotty 12:45 13:25
Posh 17:25 18:01
<end-of-file>
  
Your program shall produce:
Posh 10:10 11:00
Posh 10:15 11:10
Posh 12:05 12:30
Posh 17:25 18:01
Grotty 12:45 13:25
<end-of-file>
