[@react.component]
let make = (~name) =>
  switch (name) {
  | "play" =>
    <svg
      className="fill-current text-gray-600 w-6 h-6 mr-1" viewBox="0 0 20 20">
      <polygon points="4 4 16 10 4 16" />
    </svg>
  | "format" =>
    <svg
      className="fill-current text-gray-600 w-4 h-4 mr-1" viewBox="0 0 20 20">
      <path
        d="M1,1 L19,1 L19,3 L1,3 L1,1 Z M1,9 L19,9 L19,11 L1,11 L1,9 Z M1,17 L19,17 L19,19 L1,19 L1,17 Z M1,5 L13,5 L13,7 L1,7 L1,5 Z M1,13 L13,13 L13,15 L1,15 L1,13 Z"
      />
    </svg>
  };