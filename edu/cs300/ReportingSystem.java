package edu.cs300;

import java.io.File;
import java.io.FileWriter;
import java.io.FileNotFoundException;
import java.util.Enumeration;
import java.util.Scanner;
import java.util.Vector;
import java.util.StringTokenizer;
import java.util.concurrent.*;

class Column {
	int leftBound;
	int rightBound;
	// String leftBound;
	// String rightBound;
	String columnName;
}

class ReportThread extends Thread {

	private int threadID;
	int reportCount;	//	same as index as specified in the pdf
	String filePath;

	//	Information and report details provided by file given
	String title;
	String searchString;
	String outputFileName;
	Vector<Column> columnList = new Vector<Column>();
	Vector<String> records = new Vector<String>();

	// static {
	// 	System.loadLibrary("system5msg");
	// }


	public ReportThread(int threadID, int reportCount, String filePath) {
		this.threadID = threadID;
		this.reportCount = reportCount;
		this.filePath = filePath;
	}


	@Override
	public void run() {
		
		int index = 0;
		try {
			File file = new File(filePath);
			Scanner report = new Scanner(file);

			title = report.nextLine();
			searchString = report.nextLine();
			outputFileName = report.nextLine();

			
			String line;

			while(!((line = report.nextLine()).isEmpty())) {

				Scanner lineScanner = new Scanner(line);
				lineScanner.useDelimiter("-|,|\n");


				int left = lineScanner.nextInt();
				int right = lineScanner.nextInt();

				String columnTitle = lineScanner.next();
				Column newColumn = new Column();

				newColumn.leftBound = left;
				newColumn.rightBound = right;
				newColumn.columnName = columnTitle;


				columnList.add(newColumn); // report parameter to vector

				index++;
			


			}

	

		} catch (FileNotFoundException ex) {
			System.out.println("FileNotFoundException triggered:"+ex.getMessage());
		}

	
		try {

			MessageJNI.writeReportRequest(threadID, reportCount, searchString);


			String reportRecord;	// = MessageJNI.readReportRecord(1);

			int j = 1;
			// System.out.print(threadID + " Record: ");
			reportRecord = MessageJNI.readReportRecord(threadID);
			records.add(reportRecord);
			System.out.println("Thread " + threadID + " " + reportRecord);
			// while(reportRecord.length() > 1) {
			for(int i = 0; i < 3; i++) {
				// System.out.println(j + ": ThreadID: " + threadID + " - " + reportRecord);
				j++;
				// System.out.print(threadID + " Record: ");
				reportRecord = MessageJNI.readReportRecord(threadID);
				if(!(reportRecord.length() > 1)) {
					break;
				}
				records.add(reportRecord);

			}

		} catch (Exception e) {
			System.err.println("Error: " + e);
		}
			// }	
		// }

		


		String tempFileName = "Thread" + threadID + ".rpt"; //delete later and use the filename specified in report

		// try {
		// 	File outputFile = new File(tempFileName);
		// 	if(outputFile.exists()) {
		// 	}
		// 	if(outputFile.createNewFile()) {
		// 		// fprintf(stderr, "Report File: %s created\n" + outputFileName);
		// 		System.out.println("Report File: " + tempFileName + " created\n" + outputFileName);
		// 	} else {
		// 		System.err.println("Error: File already exists\n");
		// 		// throw new FileNotFoundException;
		// 	}
		// 	// outputFile.close();
		// } catch(Exception e) {
		// 	System.err.println("Error: " + e);
		// }

		
		if(threadID == 2) {
			System.out.println("columnList size " + columnList.size());
			String testRecord = records.get(0);
			System.out.println(testRecord);
			for(int j = 0; j < columnList.size(); j++) {
				System.out.println(j + " Left: " + columnList.get(j).leftBound + " Right: " + columnList.get(j).rightBound);
			}
			for(int i = 0; i < testRecord.length(); i++) {
				System.out.println("Index: " + i +  " character: " + testRecord.charAt(i));
			}
			String writeString = "";
			for(int j = 0; j < columnList.size(); j++) {

				writeString = writeString.concat(testRecord.substring(columnList.get(j).leftBound - 1, columnList.get(j).rightBound - 1) + "  ");
				// writeString = writeString.concat("test");
				// System.out.println(recordString.substring(columnList.get(j).leftBound - 1, columnList.get(i).rightBound - 1));
				// System.out.println("TID " + threadID+ " Left: " + columnList.get(j).leftBound + " Right: " + columnList.get(j).rightBound);

				

			}

			System.out.println(testRecord.substring(columnList.get(0).leftBound - 1, columnList.get(0).rightBound));
			System.out.println("Write String: " + writeString);
		}
		
		// write to file
		/*
		try { //test commit
			FileWriter outputFile = new FileWriter(tempFileName);	//	update later to include

			String recordString;
			String writeString;

			//	write title to file
			outputFile.write(title + "\n");

			writeString = "";
			

			//	writing column names to file
			for(int i = 0; i < columnList.size(); i++) {
				writeString = writeString.concat(columnList.get(i).columnName + "\t");
			}
			outputFile.write(writeString + "\n");


			//	
			for(int i = 0; i < records.size(); i++) {
				writeString = "";
				recordString = records.get(i);
				System.out.println("In for loop for thread " + threadID + " " + recordString);
				
				

				for(int j = 0; j < columnList.size(); j++) {

					writeString = writeString.concat(recordString.substring(columnList.get(j).leftBound - 1, columnList.get(j).rightBound) + "\t");
					// outputFile.write(recordString.substring(columnList.get(j).leftBound - 1, columnList.get(j).rightBound) + "\t");
					// writeString = writeString.concat("test");
					// System.out.println(recordString.substring(columnList.get(j).leftBound - 1, columnList.get(i).rightBound - 1));
					// System.out.println("TID " + threadID+ " Left: " + columnList.get(j).leftBound + " Right: " + columnList.get(j).rightBound);

					

				}

				outputFile.write(writeString + "\n");
				// outputFile.write("\n");
			}

			System.out.println("Output file for thread " + threadID + " written!");

			outputFile.close();



			

		} catch(Exception e) {
			System.out.println("Error: " + e);
		} */



		


	}

	public static native void writeReportRequest(int reportIdx, int reportCount, String searchString);
	public static native String readReportRecord(int qid);
	// private static native String readStringMsg();


}

public class ReportingSystem {

	// @Override
	// public void run(String filePath) {
		



	// }
	public ReportingSystem() {
		DebugLog.log("Starting Reporting System");
	}

	public int loadReportJobs() {
		int reportCounter = 0;
		try {

				// File file = new File ("/home/jfrances/f21_os_project/report_list.txt");
				File file = new File("report_list.txt");

				Scanner reportList = new Scanner(file);

				int processCount = reportList.nextInt();
				reportList.nextLine();
				reportCounter = processCount;

				for(int i = 0; i < processCount; i++) {
					if(reportList.hasNextLine()) {
						String fileName = reportList.nextLine();
						ReportThread thread = new ReportThread(i + 1, reportCounter, fileName);
						thread.start();
					} else {
						System.out.println("Error in report_list.txt file");
					}
				}

		

 		     	//	load specs and create threads for each report
				//	starting at line 2 index = 1, line 3 index = 2 ...
				// thread parses specification file ie: report1.txt, sends record request and receives records
				DebugLog.log("Load specs and create threads for each report\nStart thread to request, process and print reports");

				reportList.close();
		} catch (FileNotFoundException ex) {
				System.out.println("FileNotFoundException triggered:"+ex.getMessage());
		}
		return reportCounter;

	}

	public static void main(String[] args) throws FileNotFoundException {


			ReportingSystem reportSystem= new ReportingSystem();
			int reportCount = reportSystem.loadReportJobs();



	}


	public static native void writeReportRequest(int reportIdx, int reportCount, String searchString);
	public static native String readReportRecord(int qid);


}
