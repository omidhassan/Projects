package assignment1;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeMap;
import java.util.regex.Pattern;

public class invert {
	private static HashMap<String, Integer> map = new HashMap<String, Integer>();
	private static ArrayList<String> stopwords = new ArrayList<String>();
	private static ArrayList<String> wordsindoc = new ArrayList<String>();
	private static ArrayList<ArrayList<String>> termfreq = new ArrayList<>();
	private static ArrayList<ArrayList<String>> loc = new ArrayList<>();
	private static ArrayList<ArrayList<String>> tmparray = new ArrayList<>();
	private static HashMap<String, Integer> map2 = new HashMap<String, Integer>();
	private static Set<Entry<String, Integer>> map3 = null;
	public static void main(String[] args) throws IOException
	{
		
		String d;
		Pattern p = Pattern.compile("\\.(?!\\d)|,|:|\\(|\\)|\\$|-|;|/|\\.(?!\\d)");
		int y = 0;
		Scanner reader = null, sc = null, reader2 = null, sc2 = null;
		File fle = null, fle5 = null;
		FileWriter writer = null;
		
		try {
			fle = new File("cacm2.all");
			if(fle.createNewFile())
			{
				System.out.println("File created");
			}
			else if (fle.delete())
			{
				if (fle.createNewFile())
				{
					System.out.println("File recreated");
				}
			}
		} catch (FileNotFoundException e) {
			
			e.printStackTrace();
		}
		
		writer = new FileWriter("cacm2.all");
		
		if (args[0].equals("N"))
		{
			try {
				reader = new Scanner(new File("cacm.all"));
			} catch (FileNotFoundException e) {
				
				e.printStackTrace();
			}
			
			while (reader.hasNextLine())
			{
				
				d = reader.nextLine();
				sc = new Scanner(d);
				
				if (d.charAt(0) == '.')
				{
					if (d.charAt(1) == 'X' || d.charAt(1) == 'N' || d.charAt(1) == 'B' || d.charAt(1) == 'I')
					{
						while (reader.hasNextLine())
						{
							d = reader.nextLine();
							if ((d.charAt(0) == '.' && d.charAt(1) == 'T') || (d.charAt(0) == '.' && d.charAt(1) == 'W'))
							{
								y+=1;
								writer.write(".I" + y + "\n");
								writer.flush();
								sc = new Scanner(d);
								break;
							}
						}
					}	
					if (d.charAt(1) == 'W' && d.length() < 3)
					{
						d = reader.nextLine();
						sc = new Scanner(d);
					}
					if (d.charAt(1) == 'T' && d.length() < 3)
					{
						d = reader.nextLine();
						sc = new Scanner(d);
					}
					
				}
				sc.useDelimiter(p);
				while (sc.hasNext())
				{
					
					String s = sc.next();
					s = s.replaceAll("['\"><={}\\]\\[^*%!?|+]", "");
					
					writer.write(s.trim() + "\n");
					writer.flush();
				}
				
			}
		}
		else
		{
			stemmingalgo();
			try {
				reader = new Scanner(new File("cacm-with-stemming.all"));
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			while (reader.hasNextLine())
			{
				
				d = reader.nextLine();
				sc = new Scanner(d);
				
				if (d.charAt(0) == '.')
				{
					if (d.charAt(1) == 'x' || d.charAt(1) == 'n' || d.charAt(1) == 'b' || d.charAt(1) == 'i')
					{
						while (reader.hasNextLine())
						{
							d = reader.nextLine();
							if ((d.charAt(0) == '.' && d.charAt(1) == 't') || (d.charAt(0) == '.' && d.charAt(1) == 'w'))
							{
								y+=1;
								writer.write(".I" + y + "\n");
								writer.flush();
								sc = new Scanner(d);
								break;
							}
						}
					}	
					if (d.charAt(1) == 'w' && d.length() < 3)
					{
						d = reader.nextLine();
						sc = new Scanner(d);
					}
					if (d.charAt(1) == 't' && d.length() < 3)
					{
						d = reader.nextLine();
						sc = new Scanner(d);
					}
					
				}
				sc.useDelimiter(p);
				while (sc.hasNext())
				{
					
					String s = sc.next();
					s = s.replaceAll("['\"><={}\\]\\[^*%!?|+]", "");
					
					writer.write(s.trim() + "\n");
					writer.flush();
				}
				
			}
		}
		
		writer.flush();
		writer.close();
		
		if (args[1].equals("Y"))
		{
			fillstopwords();
		}
		
		
		createDictionary();
		postingsfile();
	}
	public static void stemmingalgo() throws IOException
	{
		String[] arguments = new String[] {"cacm.all"};
		Stemmer.main(arguments);
	}
	
	public static void createDictionary() throws IOException
	{
		
		File fle2= null;
		Scanner scn = new Scanner(new File("cacm2.all")), read = null;
		String ln, s, docid = null , term = null;;
		int t = 0, q=0, num = 0, r2 = 0;;
		
		try {
			
			fle2 = new File("dictionary.all");
			if(fle2.createNewFile())
			{
				System.out.println("File created");
			}
			else if (fle2.delete())
			{
				if (fle2.createNewFile())
				{
					System.out.println("File recreated");
				}
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		while (scn.hasNextLine())
		{
			ln = scn.nextLine();
			read = new Scanner(ln);
			num+=1;
			
			while (read.hasNext())
			{
				//s = read.next();
				if (!read.hasNextInt() && !ln.equals(""))
				{
					s = read.next().toLowerCase();
					term = s +  " " + docid;
					if (!stopwords.contains(s) && s.length() > 1 && !(s.charAt(0) == '.' && s.charAt(1) == 'i'))
					{
						if(map.containsKey(s) && !wordsindoc.contains(s))
						{
							map.put(s, map.get(s) + 1);
							if(map2.containsKey(term))
							{
								map2.put(term, map2.get(term) + 1);
								for (q = 0; q < loc.size(); q++)
								{
									if (loc.get(q).contains(term))
									{
										loc.get(q).add(num + "");
										
									}
								}
							}
							else
							{
								map2.put(term, 1);
								loc.add(new ArrayList());
								loc.get(r2).add(term);
								loc.get(r2).add(num + "");
								
								r2+=1;
							}
							
							for (q = 0; q < termfreq.size(); q++)
							{
								if (termfreq.get(q).contains(s))
								{
									termfreq.get(q).add(docid);
								}
							}
							
							wordsindoc.add(s);	
						}
						else if(!wordsindoc.contains(s))
						{
							map.put(s, 1);
							map2.put(term, 1);
							
							termfreq.add(new ArrayList());
							termfreq.get(t).add(s);
							termfreq.get(t).add(docid);
							
							loc.add(new ArrayList());
							loc.get(r2).add(term);
							loc.get(r2).add(num + "");
							
							t+=1;
							r2+=1;
							
							wordsindoc.add(s);
							
						}
						else if (map.containsKey(s))
						{
							map2.put(term, map2.get(term) + 1);
							for (q = 0; q < loc.size(); q++)
							{
								if (loc.get(q).contains(term))
								{
									loc.get(q).add(num + "");
									
								}
							}
						}
					}
					else if (s.charAt(0) == '.' && s.charAt(1) == 'i')
					{
						docid = s.replaceAll("[^0-9]", "");
						
						wordsindoc.clear();
						num = 0;
					}
				}
				else
				{
					read.next();
				}
				
			}
			
		}
		
		TreeMap<String, Integer> sortedmap = new TreeMap<String, Integer>(map);
		map3 = sortedmap.entrySet();
		try (FileWriter writer3 = new FileWriter("dictionary.all")) {
			String h = "\tWord\tDocument Frequency\n";
			writer3.write(h);
			writer3.flush();
			for(Entry<String, Integer> mapping : map3)
			{
				writer3.write(String.format("-%10s", mapping.getKey())+ String.format("%10s", mapping.getValue()) + "\n");
				writer3.flush();
			}
			writer3.close();
	    }
		
	}
	
	public static void postingsfile() throws IOException
	{
		String result, docid, tmp, location = "";
		int termf, tmpnum = 1;
		boolean found = false, found2 = false;
		File fle3 = null;
		
		try {
			
			fle3 = new File("Postings.txt");
			if(fle3.createNewFile())
			{
				System.out.println("File created");
			}
			else if (fle3.delete())
			{
				if (fle3.createNewFile())
				{
					System.out.println("File recreated");
				}
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Collections.sort(termfreq, new Comparator<ArrayList<String>>() {    
	        @Override
	        public int compare(ArrayList<String> x1, ArrayList<String> x2) {
	            return x1.get(0).compareTo(x2.get(0));
	        }               
		});
		Collections.sort(loc, new Comparator<ArrayList<String>>() {    
	        @Override
	        public int compare(ArrayList<String> x1, ArrayList<String> x2) {
	            return x1.get(0).compareTo(x2.get(0));
	        }               
		});
		tmparray = (ArrayList<ArrayList<String>>) termfreq.clone();
		
			PrintStream filestream = new PrintStream("Postings.txt");
			for(Entry<String, Integer> mapping : map3)
			{
				
				filestream.print(tmpnum + ". ");
				filestream.flush();
				for (int i = 0; i < termfreq.size(); i++)
				{
					int innerarray = termfreq.get(i).size();
				
					for (int g = 1; g < innerarray; g++)
					{

						if (termfreq.get(i).get(0).equals(mapping.getKey()))
						{
							docid = termfreq.get(i).get(g);
							tmp = mapping.getKey() + " " + docid;
							termf = map2.get(tmp);
							found = true;
							for (int c = 0; c < loc.size(); c++)
							{
								int innerarray2 = loc.get(c).size();
								location = "";
								for (int h = 1; h < innerarray2; h++)
								{
									if (loc.get(c).get(0).equals(tmp))
									{
										
										found2 = true;
										
										location = location + loc.get(c).get(h) + ",";
										
									}
									else if (!(loc.get(c).get(h-1).equals(mapping.getKey())))
									{
										
										break;
									}
								}
								if (found2 == true)
								{
									loc.remove(c);
									found2 = false;
									break;
								}
							}
							
						    filestream.print("[" + docid + "][" + termf + "][" + location + "]  ");
						    filestream.flush();
	 						
						}
						else if (!(termfreq.get(i).get(g-1).equals(mapping.getKey())))
						{
							break;
						}
					}
					if (found == true)
					{
						termfreq.remove(i);
						found = false;
						location = "";
						break;
					}
				}
				tmpnum+=1;
			
		
			
			filestream.println("");
		    filestream.flush();
			
		}
		
	}
	
	
	public static void fillstopwords()
	{
		Scanner scn = null;
		try {
			scn = new Scanner(new File("stopwords.txt"));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String stpword;
		
		while (scn.hasNextLine())
		{
			stpword = scn.nextLine();
			stopwords.add(stpword);
		}
	}
	public static ArrayList<String> wordsdocid (String word)
	{
		for (int x = 0; x < tmparray.size(); x++)
		{
			if (tmparray.get(x).get(0).equals(word))
			{
				return tmparray.get(x);
			}
		}
		return new ArrayList<>();
		
	}
}
