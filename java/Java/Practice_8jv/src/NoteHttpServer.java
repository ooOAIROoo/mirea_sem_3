import java.io.*;
import java.net.*;
import java.util.*;
public class NoteHttpServer {
    private static final int PORT = 8080;
    private static final List<String> notes = new ArrayList<>();
    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Note HTTP Server запущен на порту " + PORT);
            while (true) {
                try (Socket clientSocket = serverSocket.accept();
                     PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
                     BufferedReader in = new BufferedReader(new
                             InputStreamReader(clientSocket.getInputStream()))) {
                    String inputLine;
                    StringBuilder request = new StringBuilder();
                    while ((inputLine = in.readLine()) != null && !inputLine.isEmpty()) {
                        request.append(inputLine).append("\n");
                    }
                    String[] requestLines = request.toString().split("\n");
                    String method = requestLines[0].split(" ")[0];
                    String path = requestLines[0].split(" ")[1];
                    String response;
                    if (method.equals("GET") && path.equals("/notes")) {
                        response = "HTTP/1.1 200 OK\r\n" +
                                "Content-Type: text/html\r\n" +
                                "\r\n" +
                                "<html><body><h1>Notes</h1><ul>";
                        for (String note : notes) {
                            response += "<li>" + note + "</li>";
                        }
                        response += "</ul></body></html>";
                    } else if (method.equals("POST") && path.equals("/add")) {
                        String note = requestLines[requestLines.length - 1].trim();
                        if (!note.isEmpty()) {
                            notes.add(note);
                            response = "HTTP/1.1 200 OK\r\n" +
                                    "Content-Type: text/html\r\n" +
                                    "\r\n" +
                                    "<html><body><h1>Note added</h1></body></html>";
                        } else {
                            response = "HTTP/1.1 400 Bad Request\r\n" +
                                    "Content-Type: text/html\r\n" +
                                    "\r\n" +
                                    "<html><body><h1>Empty note</h1></body></html>";
                        }
                    } else if (method.equals("POST") && path.equals("/remove")) {
                        if (!notes.isEmpty()) {
                            notes.remove(notes.size() - 1);
                            response = "HTTP/1.1 200 OK\r\n" +
                                    "Content-Type: text/html\r\n" +
                                    "\r\n" +
                                    "<html><body><h1>Last note removed</h1></body></html>";
                        } else {
                            response = "HTTP/1.1 400 Bad Request\r\n" +
                                    "Content-Type: text/html\r\n" +
                                    "\r\n" +
                                    "<html><body><h1>No notes to remove</h1></body></html>";
                        }
                    } else {
                        response = "HTTP/1.1 404 Not Found\r\n" +
                                "Content-Type: text/html\r\n" +
                                "\r\n" +
                                "<html><body><h1>404 Not Found</h1></body></html>";
                    }
                    out.print(response);
                    out.flush();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
