/* service-worker.js */

const CACHE_NAME = "SmartTankCache-v1";

// Cache only static files — NOT Firebase or ThingSpeak
const FILES_TO_CACHE = [
  "./",
  "./index.html",
  "./manifest.json",
  "./icons/icon-192.png",
  "./icons/icon-512.png"
];

self.addEventListener("install", (event) => {
  event.waitUntil(
    caches.open(CACHE_NAME).then((cache) => {
      return cache.addAll(FILES_TO_CACHE);
    })
  );
  self.skipWaiting();
});

self.addEventListener("activate", (event) => {
  event.waitUntil(
    caches.keys().then((keys) => {
      return Promise.all(
        keys.map((key) => {
          if (key !== CACHE_NAME) {
            return caches.delete(key);
          }
        })
      );
    })
  );
  self.clients.claim();
});

self.addEventListener("fetch", (event) => {
  const url = event.request.url;

  // Do NOT cache Firebase or ThingSpeak or API calls
  if (
    url.includes("firebaseio") ||
    url.includes("gstatic") ||
    url.includes("googleapis") ||
    url.includes("api.thingspeak.com")
  ) {
    return;
  }

  // For static files → Cache-first
  event.respondWith(
    caches.match(event.request).then((cached) => {
      return (
        cached ||
        fetch(event.request)
          .then((response) => {
            // Clone response for caching
            const cloned = response.clone();
            caches.open(CACHE_NAME).then((cache) => {
              cache.put(event.request, cloned);
            });
            return response;
          })
          .catch(() => {
            // Offline fallback for missing files
            return caches.match("./index.html");
          })
      );
    })
  );
});
