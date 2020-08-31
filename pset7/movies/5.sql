SELECT movies.title AS 'Movie Title', movies.year AS 'Year of Release'
FROM movies
WHERE movies.title LIKE 'Harry Potter%'
ORDER BY movies.year ASC;
