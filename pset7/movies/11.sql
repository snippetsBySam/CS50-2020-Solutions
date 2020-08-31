SELECT movies.title FROM movies
JOIN ratings ON ratings.movie_id = movies.id
WHERE ratings.movie_id IN(SELECT stars.movie_id FROM stars WHERE stars.person_id = (SELECT people.id FROM people WHERE people.name = 'Chadwick Boseman'))
ORDER BY ratings.rating DESC
LIMIT 5;
