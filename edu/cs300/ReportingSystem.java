package edu.cs300;

import java.io.File;
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
				columnList.add(newColumn);

				index++;
			


			}
				
			// 	System.out.println(columnList.get(i).columnName + " " + columnList.get(i).leftBound + " " + columnList.get(i).rightBound);
			// 	System.out.println("ThreadID: " + threadID);
			// }
			

		} catch (FileNotFoundException ex) {
			System.out.println("FileNotFoundException triggered:"+ex.getMessage());
		}
		
		// if(threadID == 1) {

			
			// for(int i = 0; i < index; i++) {

		if(true) {	//eliminate later
			try {
				
				// MessageJNI.writeReportRequest(5, 1,"test");
				// System.out.println(MessageJNI.readReportRecord(1));
				MessageJNI.writeReportRequest(threadID, reportCount, searchString);

				// System.out.println(MessageJNI.readReportRecord(1));
				// System.out.println(MessageJNI.readReportRecord(1));
				// MessageJNI.readReportRecord(1);
				// MessageJNI.readReportRecord(1);
				String reportRecord;	// = MessageJNI.readReportRecord(1);

				// System.out.println("String length: " + reportRecord.length());

				// String reportRecord = "";
				int j = 1;

				reportRecord = MessageJNI.readReportRecord(threadID);
				while(reportRecord.length() > 1) {
					System.out.println(j + ": ThreadID: " + threadID + " - " + reportRecord);
					j++;
					reportRecord = MessageJNI.readReportRecord(threadID);

				}
				// MessageJNI.readReportRecord(1);

			} catch (Exception e) {
				System.err.println("Error: " + e);
			}
			// }	
		}

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
