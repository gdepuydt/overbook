use std::fs::read_to_string;


struct Mistake<'a> {
    location: &'a str,
}

fn main() -> Result<(), std::io::Error> {
    
    let path = "sample.txt";
    let mut s = String::with_capacity(256 * 1024);
    let result = check(path, &mut s)?;
    if let Some(mistake) = result {
        println!("mistake: commas are forbidden: {:?}", mistake.location);
    }

    Ok(())
}

use std::io::Error as E;

fn check<'a>(path: &str, s: &'a mut String) -> Result<Option<Mistake<'a>>, E> {
    
    use std::fs::File;
    use std::io::Read;
    
    s.clear();
    File::open(path)?.read_to_string(s)?;

    println!("~ {} ~", path);

    Ok(match s.find(",") {
        Some(index) => {
            let location = &s[index..];
            Some(Mistake {location})
        }
        None => None,
    })

}

// TODO(Geert): A little more structure