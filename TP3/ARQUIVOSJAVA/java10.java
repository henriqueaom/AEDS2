import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Scanner;

public class java10 {

    // Inner class to represent a Show
    static class Show {
        private String showId;
        private String type;
        private String title;
        private String director;
        private String[] cast;
        private String country;
        private Date dataAdded; // Using java.util.Date as in your provided code
        private int releaseYear;
        private String rating;
        private String duration;
        private String listedIn;
        private String description;

        // Constructor
        public Show() {
            // Initialize default values similar to C struct initialization
            this.showId = "NaN";
            this.type = "NaN";
            this.title = "NaN";
            this.director = "NaN";
            this.cast = new String[0]; // Empty array for no cast
            this.country = "NaN";
            this.dataAdded = new Date(0); // Epoch start for "NaN" dates
            this.releaseYear = 0;
            this.rating = "NaN";
            this.duration = "NaN";
            this.listedIn = "NaN";
            this.description = "NaN";
        }

        // Method to parse a CSV line and populate this Show object
        public void ler(String linha) {
            List<String> campos = parseLinhaCSV(linha);
            // Define the date format (e.g., "November 19, 2019")
            SimpleDateFormat sdf = new SimpleDateFormat("MMMM d, yyyy", Locale.ENGLISH);

            this.showId = getOrNaN(campos, 0);
            this.type = getOrNaN(campos, 1);
            this.title = getOrNaN(campos, 2);
            this.director = getOrNaN(campos, 3);

            String rawCast = getOrNaN(campos, 4);
            // Split cast string by comma and optional whitespace
            this.cast = rawCast.equals("NaN") ? new String[0] : rawCast.split(",\\s*");
            // Remove leading/trailing quotes from cast members if any
            for (int i = 0; i < this.cast.length; i++) {
                this.cast[i] = this.cast[i].replaceAll("^\"|\"$", "");
            }

            this.country = getOrNaN(campos, 5);

            try {
                String dateStr = getOrNaN(campos, 6);
                // Parse date string, or set to epoch if "NaN"
                this.dataAdded = dateStr.equals("NaN") ? new Date(0) : sdf.parse(dateStr);
            } catch (ParseException e) {
                // If parsing fails, default to epoch start (similar to C's 0 for time_t)
                this.dataAdded = new Date(0);
            }

            String yearStr = getOrNaN(campos, 7);
            try {
                // Parse release year, or set to 0 if "NaN" or invalid
                this.releaseYear = yearStr.equals("NaN") ? 0 : Integer.parseInt(yearStr);
            } catch (NumberFormatException e) {
                this.releaseYear = 0;
            }

            this.rating = getOrNaN(campos, 8);
            this.duration = getOrNaN(campos, 9);
            this.listedIn = getOrNaN(campos, 10);
            this.description = getOrNaN(campos, 11);
        }

        // Helper to get field value or "NaN" if empty or out of bounds
        private String getOrNaN(List<String> fields, int index) {
            if (index < fields.size() && !fields.get(index).trim().isEmpty()) {
                String field = fields.get(index).trim();
                // Remove surrounding quotes from the field if present
                if (field.length() > 1 && field.charAt(0) == '"' && field.charAt(field.length() - 1) == '"') {
                    return field.substring(1, field.length() - 1);
                }
                return field;
            }
            return "NaN";
        }

        // Custom CSV line parser to handle commas within quoted fields
        private List<String> parseLinhaCSV(String linha) {
            List<String> campos = new ArrayList<>();
            boolean inQuotes = false;
            StringBuilder sb = new StringBuilder();

            for (int i = 0; i < linha.length(); i++) {
                char c = linha.charAt(i);
                if (c == '\"') {
                    inQuotes = !inQuotes;
                    // Do not append quote if it's a delimiter, only if it's part of content
                    // For example, "Tom ""The Cat"" Jones" should be Tom "The Cat" Jones
                    // This simple parser assumes no escaped quotes inside fields, just delimiters
                } else if (c == ',' && !inQuotes) {
                    campos.add(sb.toString().trim());
                    sb.setLength(0); // Clear the buffer
                } else {
                    sb.append(c);
                }
            }
            campos.add(sb.toString().trim()); // Add the last field
            return campos;
        }

        // Getter for dataAdded for comparison
        public Date getDataAdded() {
            return dataAdded;
        }

        // Getter for title for comparison
        public String getTitle() {
            return title;
        }

        // Method to format the Show object into the desired output string
        public String formatar() {
            // Create a mutable copy and sort the cast array
            String[] sortedCast = Arrays.copyOf(cast, cast.length);
            Arrays.sort(sortedCast);

            // Format cast string (e.g., "Actor1, Actor2, NaN")
            String castFormatted = sortedCast.length > 0 ? String.join(", ", sortedCast) : "NaN";

            // Format date for output
            String dateFormatted;
            if (dataAdded != null && dataAdded.getTime() != 0) { // Check for non-null and non-epoch date
                SimpleDateFormat outputSdf = new SimpleDateFormat("MMMM d, yyyy", Locale.ENGLISH);
                dateFormatted = outputSdf.format(dataAdded);
            } else {
                dateFormatted = "NaN";
            }

            // Construct the final output string
            return String.format("=> %s ## %s ## %s ## %s ## [%s] ## %s ## %s ## %d ## %s ## %s ## [%s] ##",
                    showId,
                    title,
                    type,
                    director,
                    castFormatted,
                    country,
                    dateFormatted,
                    releaseYear,
                    rating,
                    duration,
                    listedIn
            );
        }
    } // End of Show class

    // Store all shows by ID for quick lookups
    private static Map<String, Show> allShowsMap = new HashMap<>();

    // Function to load all CSV data into the map
    public static void loadCsvData(String filePath) {
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            br.readLine(); // Skip header line
            String line;
            while ((line = br.readLine()) != null) {
                Show s = new Show();
                s.ler(line);
                allShowsMap.put(s.showId, s);
            }
        } catch (IOException e) {
            System.err.println("Error reading CSV file: " + e.getMessage());
        }
    }

    // Main method
    public static void main(String[] args) throws Exception {
        // Redirect System.out to log file and console
        String logFileName = "202400000_quicksort3.txt"; // Replace 202400000 with your actual student ID
        try (PrintWriter logWriter = new PrintWriter(new FileWriter(logFileName))) {
            // Custom PrintStream to write to both console and log file
            System.setOut(new java.io.PrintStream(System.out) {
                @Override
                public void println(String s) {
                    super.println(s); // Print to console
                    logWriter.println(s); // Write to log file
                }
            });
            // Ensure any direct System.err also goes to log if needed (optional)
            // System.setErr(new java.io.PrintStream(System.err) {
            //     @Override
            //     public void println(String s) {
            //         super.println(s);
            //         logWriter.println("[ERROR] " + s);
            //     }
            // });

            loadCsvData("/tmp/disneyplus.csv"); // Load all data once

            Scanner sc = new Scanner(System.in);
            List<Show> selectedShows = new ArrayList<>();
            String id;

            // First part: read show IDs from standard input until "FIM"
            while (sc.hasNextLine()) {
                id = sc.nextLine().trim();
                if (id.equalsIgnoreCase("FIM")) {
                    break;
                }
                Show show = allShowsMap.get(id); // Get reference from the map
                if (show != null) {
                    selectedShows.add(show);
                }
            }

            // Sort the selected shows based on dataAdded then title
            Collections.sort(selectedShows, new Comparator<Show>() {
                @Override
                public int compare(Show s1, Show s2) {
                    // Primary sort key: dataAdded
                    // Handle null/epoch dates (0L)
                    long time1 = (s1.dataAdded != null) ? s1.dataAdded.getTime() : 0L;
                    long time2 = (s2.dataAdded != null) ? s2.dataAdded.getTime() : 0L;

                    int dateComparison = Long.compare(time1, time2);

                    if (dateComparison != 0) {
                        return dateComparison;
                    }

                    // Secondary sort key: title (if dates are equal)
                    return s1.title.compareTo(s2.title);
                }
            });

            // Print the sorted shows
            for (Show show : selectedShows) {
                System.out.println(show.formatar());
            }

            sc.close(); // Close the scanner
        } catch (IOException e) {
            System.err.println("Error initializing log file: " + e.getMessage());
            e.printStackTrace();
        }
    }
}