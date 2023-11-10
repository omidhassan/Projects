package assignment1;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.regex.Pattern;

public class test {
	private static ArrayList<String> docid = new ArrayList<String>();
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner userinput = new Scanner(System.in);
		System.out.println("Would you like to run this program with the stemming algorthim (Y/N)");
		String ans = userinput.nextLine();
		System.out.println("Would you like to run the invert program with stop words (Y/N)");
		String ans2 = userinput.nextLine();
		String[] arguments = new String[] {ans, ans2};
		try {
			invert.main(arguments);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("Enter a word below");
		String input, ln, word, ln2 = null, ln3;
		int dictlinenum = 0;
		boolean found = false, found2 = false, found3 = false, foundfirst = false;
		Pattern p = Pattern.compile("\\.(?!\\d)|,|:|\\(|\\)|\\$|-|;|/|\\.(?!\\d)");
		while (!((input = userinput.nextLine()).equals("ZZEND")))
		{
			Scanner readerdict = null, readerpost = null, sc = null, sc2 = null, readercacm = null, sc3 = null;
			dictlinenum = 0;
			try {
				readerdict = new Scanner(new File("dictionary.all"));
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				readerpost = new Scanner(new File("Postings.txt"));
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if (ans.equals("N"))
			{
				try {
					readercacm = new Scanner(new File("cacm.all"));
				} catch (FileNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			else
			{
				try {
					readercacm = new Scanner(new File("cacm-with-stemming.all"));
				} catch (FileNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			while (readerdict.hasNextLine())
			{
				ln = readerdict.nextLine();
				sc = new Scanner(ln);
				while (sc.hasNext())
				{
					word = sc.next();
					if (word.equals(input.toLowerCase()))
					{
						found = true;
					}
				}
				if (found)
				{
					found = false;
					break;
				}
				dictlinenum+=1;
			}
			while (readerpost.hasNextLine())
			{
				ln2 = readerpost.nextLine();
				sc2 = new Scanner(ln2);
				while (sc2.hasNext())
				{
					word = sc2.next();
					//System.out.println(word);
					if (word.equals(dictlinenum + "."))
					{
						//System.out.println(ln2);
						found2 = true;
					}
				}
				if (found2)
				{
					found2 = false;
					break;
				}
			}
			docid = (ArrayList<String>) invert.wordsdocid(input).clone();
			ArrayList<Long> time = new ArrayList<Long>();
			String[] split2 = ln2.split("\\s+");

			String title = null;
			int num1 = 1;
			final long start1 = System.currentTimeMillis();
			//final long start2 = System.currentTimeMillis();
			while (readercacm.hasNextLine() && !(docid.size() == num1))
			{
				ln3 = readercacm.nextLine().trim().toLowerCase();
				readercacm.useDelimiter(p);
				sc3 = new Scanner(ln3);
				if (ln3.equals(".i " + docid.get(num1)) || found3)
				{
					found3 = true;
					if (ln3.equals(".t"))
					{
						ln3 = readercacm.nextLine().trim().toLowerCase();
						sc3 = new Scanner(ln3);
						title = ln3;
					}
					while (sc3.hasNext())
					{
						sc3.useDelimiter(p);
						word = sc3.next().toLowerCase();
						word = word.replaceAll("['\"><={}\\]\\[^*%!?|+]", "");
						String[] split3 = word.split("\\s+");
						for (String word2 : split3)
						{
							if (word2.equals(input))
							{
								System.out.println(split2[num1]);
								System.out.println("Doc Title: " + title);
								System.out.println("Context: " + ln3);
								foundfirst = true;
								num1+=1;
								final long end1 = System.currentTimeMillis();
								long time1 = end1 - start1;
								time.add(time1);
								System.out.println("Excution time for this occurence: " + time1);
								break;
							}
						}
						if (foundfirst)
						{
							break;
						}
					}
					if (foundfirst)
					{
						foundfirst = false;
						found3 = false;
						
					}
				}
			}
			long avgtime = 0; 
			for (int k = 0; k < time.size(); k++)
			{
				avgtime = avgtime + time.get(k);
			}
			System.out.println("Average Time of excution is: " + (avgtime/time.size()));
			//final long end2 = System.currentTimeMillis();
			
		}
		
		
	}

}
